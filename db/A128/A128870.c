/*
 * A128870  Numbers n that are divisible by d, where a_i for 1 <= i <= k are
 *          the digits of n and d = Product_{i=1..k}Sum_of_digits_of_(a_i^k).
 * https://oeis.org/A128870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   append_candidate, big_pow, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A128870: numbers n such that n is divisible by
 * d = ∏_{i=1..k} (sum of digits of a_i^k),
 * where a_i are the decimal digits of n and k is the number of digits.
 * The function generates the sequence by scanning integers in order and
 * storing the ones that satisfy the condition. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/digit_sum.h"
#include "../utils/append_candidate.h"

#define MAX_DIGITS 2000                       /* maximum digit count pre‑computed */

static uint16_t sum_pow[MAX_DIGITS + 1][10] = {{0}};
static size_t  sum_pow_max = 0;               /* maximum k for which sum_pow is ready */

static void ensure_sum_pow(size_t k)
{
    if (k <= sum_pow_max) return;
    if (k > MAX_DIGITS) return;              /* safety guard */
    for (size_t cur = sum_pow_max + 1; cur <= k; ++cur) {
        for (int d = 0; d < 10; ++d) {
            sum_pow[cur][d] = (uint16_t)digit_sum(big_pow((seq_t)d, (seq_t)cur));
        }
    }
    sum_pow_max = k;
}

static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t  next_candidate = 1;            /* next integer to test */

static void test_candidate(idx_t cand)
{
    /* extract digits of cand into an array */
    idx_t   tmp   = cand;
    uint8_t dig[32];
    size_t  len   = 0;
    do {
        dig[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    /* numbers containing a zero digit cannot satisfy the condition */
    for (size_t i = 0; i < len; ++i)
        if (dig[i] == 0) return;

    ensure_sum_pow(len);

#if defined(__SIZEOF_INT128__)
    unsigned __int128 prod   = 1;
    unsigned __int128 cand128 = (unsigned __int128)cand;

    for (size_t i = 0; i < len; ++i) {
        uint16_t s = sum_pow[len][dig[i]];
        if (s == 0) return;
        if (prod > cand128 / s) return;          /* product would exceed candidate */
        prod *= s;
    }
    if (cand128 % prod == 0) {
        append_candidate(&seq_cache, &cache_len, &cache_cap, (seq_t)cand);
    }
#else
    seq_t prod    = 1;
    seq_t cand_seq = (seq_t)cand;

    for (size_t i = 0; i < len; ++i) {
        seq_t s = (seq_t)sum_pow[len][dig[i]];
        if (s == 0) return;
        if (prod > cand_seq / s) return;          /* product would exceed candidate */
        prod *= s;
    }
    if (cand_seq % prod == 0) {
        append_candidate(&seq_cache, &cache_len, &cache_cap, (seq_t)cand);
    }
#endif
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    while (cache_len < (size_t)n) {
        test_candidate(next_candidate);
        ++next_candidate;
    }
    return seq_cache[(size_t)n - 1];
}
