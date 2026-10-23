/*
 * A042996  Numbers k such that binomial(k, floor(k/2)) is divisible by k.
 * https://oeis.org/A042996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer k such that * binom(k,
 *            floor(k/2)) is divisible by k. * Computation: brute‑force search
 *            with Legendre exponent checks
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool is_good(uint64_t k)
{
    if (k == 0) return false;
    if (k & 1ULL) return true;          /* all odd numbers work */

    uint64_t m = k >> 1;                 /* k = 2*m */

    uint64_t tmp = k;
    while (tmp > 1) {
        /* factor out one prime divisor */
        uint64_t p = 0;
        for (uint64_t d = 2; d * d <= tmp; ++d) {
            if (tmp % d == 0) { p = d; break; }
        }
        if (p == 0) p = tmp;             /* tmp is prime */

        uint64_t exp = 0;
        while (tmp % p == 0) { tmp /= p; ++exp; }

        /* Legendre exponent for binom(2*m, m) */
        uint64_t vp = 0;
        uint64_t power = p;
        while (power <= 2 * m) {
            vp += (2 * m) / power - 2 * (m / power);
            if (power > (UINT64_MAX / p)) break;  /* prevent overflow */
            power *= p;
        }
        if (vp < exp) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t last_k_checked = 0;

    if (terms_cap == 0) {
        terms_cap = 128;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    while (terms_len < n) {
        ++last_k_checked;
        if (is_good(last_k_checked)) {
            if (terms_len == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)last_k_checked;
        }
    }
    return terms[n - 1];
}
