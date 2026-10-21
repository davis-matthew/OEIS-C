/*
 * A072185  Primes p = x1x2x3...xn in base 10 such that x1^p1 + x2^p2 + x3^p3
 *          + ... + xn^pn is a prime, where pn is the n-th prime.
 * https://oeis.org/A072185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 37, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow, get_prime, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A072185: primes p = x1x2…xn such that
   ∑i=1..n x_i^{p_i} is prime, where p_i is the i‑th prime.
   a(n) is returned for n ≥ 1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t   cache_len = 0;
    static size_t   cache_cap = 0;
    static idx_t    candidate = 2;

    if (n == 0) return 0;

    while (cache_len < (size_t)n) {
        /* Find the next prime candidate. */
        while (!is_prime(candidate))
            ++candidate;

        /* Extract the decimal digits of the candidate (most‑significant first). */
        int d[64];
        int dlen = 0;
        idx_t tmp = candidate;
        if (tmp == 0) {
            d[dlen++] = 0;
        } else {
            while (tmp > 0) {
                d[dlen++] = (int)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Compute the sum Σ x_i^{p_i}. */
        seq_t sum = 0;
        for (int i = 0; i < dlen; ++i) {
            int digit = d[dlen - 1 - i];                     /* most‑significant first */
            seq_t term   = big_pow((seq_t)digit,
                                    (seq_t)get_prime((idx_t)(i + 1)));
            sum += term;
        }

        /* If the sum is prime, store the candidate. */
        if (is_prime_big(sum)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 4;
                seq_t *tmp_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_arr) abort();
                cache = tmp_arr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }

        ++candidate;  /* advance to the next integer */
    }

    return cache[n - 1];
}
