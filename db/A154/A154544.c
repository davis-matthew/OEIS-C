/*
 * A154544  Primes p such that the smallest digit of p + the number of
 *          smallest digits of p is an even composite number.
 * https://oeis.org/A154544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 966, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 983 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that the smallest digit of p plus the
 *   number of its occurrences is an even composite number. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static uint64_t next_candidate = 1;   /* small integer to test for primality */
static size_t seq_cap = 0;             /* capacity of cached array */
static size_t seq_len = 0;             /* number of cached terms */
static seq_t *seq_vals = NULL;         /* cache of sequence values */

/* Ensure the cache has at least 'needed' slots. */
static void grow_seq_array(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = (seq_cap > 0) ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = mem_resize(seq_t, seq_vals, new_cap);
    if (!new_arr)
        abort();
    seq_vals = new_arr;
    seq_cap = new_cap;
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* Generate terms until we have the nth one. */
    while (seq_len < (size_t)n) {
        /* Find next prime candidate. */
        do {
            ++next_candidate;
        } while (!is_prime_big((seq_t)next_candidate));

        seq_t p = (seq_t)next_candidate;

        /* Determine the smallest digit and its count. */
        seq_t tmp = p;
        seq_t min_digit = 10;   /* greater than any decimal digit */
        while (tmp > 0) {
            seq_t d = tmp % 10;
            if (d < min_digit)
                min_digit = d;
            tmp /= 10;
        }

        tmp = p;
        seq_t count = 0;
        while (tmp > 0) {
            seq_t d = tmp % 10;
            if (d == min_digit)
                ++count;
            tmp /= 10;
        }

        seq_t sum = min_digit + count;
        /* Check if sum is even composite. */
        if ((sum % 2 == 0) && (sum > 2) && !is_prime_seq(sum)) {
            grow_seq_array(seq_len + 1);
            seq_vals[seq_len++] = p;
        }
    }

    return seq_vals[(size_t)n - 1];
}
