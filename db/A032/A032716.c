/*
 * A032716  Numbers k such that k prefixed by '8' and followed by '3' is
 *          prime.
 * https://oeis.org/A032716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   digit_count, grow_seq, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th integer k such that the decimal number
   formed by prefixing k with '8' and suffixing it with '3' is prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t last_k_checked = 1;   // next candidate to test
static idx_t found_count = 0;       // number of values stored in cache

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset not used */

    /* Ensure the cache can hold at least n elements. */
    if (cache_cap < (size_t)n)
        grow_seq(&cache, &cache_cap, (size_t)n);

    /* Generate new terms until we have at least n. */
    while ((idx_t)found_count < n) {
        idx_t k = last_k_checked;
        ++last_k_checked;

        /* Number N = 8 * 10^(digits(k)+1) + k*10 + 3. */
        idx_t d    = digit_count(k);          /* number of digits of k */
        seq_t pow  = pow10((idx_t)(d + 1));    /* 10^(d+1) */
        seq_t N    = (seq_t)8 * pow + (seq_t)k * 10 + 3;

        if (is_prime_big(N))
            cache[found_count++] = (seq_t)k;
    }

    return cache[n - 1];
}
