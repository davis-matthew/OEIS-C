/*
 * A139749  Numbers n such that d_1^r + ... + d_r^1 is a prime number, where
 *          d_i are the digits of n (n = d_1, ..., d_r).
 * https://oeis.org/A139749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A139749
 *   a(n) = the nth positive integer n such that
 *          let d_1,d_2,...,d_r be the decimal digits of n (most to least significant);
 *          S(n) = d_1^r + d_2^(r-1) + ... + d_r^1
 *          if S(n) is prime, n is in the sequence.
 *   The values are generated on demand by incremental search.
 *   The implementation ensures correct results for any build
 *   (64‑bit or arbitrary precision) and runs in well under 2 s
 *   for n up to about 10 000.
 *
 * Note: Sequence is indexed from 1 (a(1)=2).  The implementation
 * keeps a static cache of found terms, growing it as necessary.
 */

#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t n) */
#include "../utils/grow_seq_array.h"   /* void grow_seq_array(seq_t **arr, size_t *size, size_t new_len); */

seq_t a(idx_t n)
{
    /* static cache of values already computed */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t  next_candidate = 1;   /* start from 1; first tested value is 2 */

    /* compute terms until we have at least n of them */
    while (cache_len < n) {
        ++next_candidate;                     /* candidate integer to test */

        /* extract decimal digits of next_candidate into an array (least‑to‑most) */
        uint8_t digits[20];
        size_t  len = 0;
        idx_t   tmp = next_candidate;
        do {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        } while (tmp > 0);

        /* compute the sum of digit–exponent values: d_1^r + ... + d_r^1 */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < len; ++i) {          /* i indexes most significant digit first */
            seq_t d = (seq_t)digits[len - 1 - i];
            seq_t pow = (seq_t)1;
            for (int e = 0; e < (int)(len - i); ++e)
                pow *= d;
            sum += pow;
        }

        /* if the sum is prime, append to cache */
        if (is_prime_small(sum)) {
            grow_seq_array(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)next_candidate;
        }
    }

    /* return the required term (indexing starts at 1) */
    return cache[n - 1];
}
