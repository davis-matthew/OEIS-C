/*
 * A081370  Numbers k such that binomial(k^2, k) reduced mod k^2 is 0.
 * https://oeis.org/A081370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1326, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS sequence A081370.
   A term k satisfies binomial(k², k) ≡ 0 (mod k²).
   The function enumerates k in increasing order, caching the results,
   and returns the nth term (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Count p‑adic valuation of n!  (n is an unsigned integer). */
static idx_t v_p_factorial(idx_t n, uint64_t p)
{
    idx_t count = 0;
    unsigned __int128 power = p;
    while (power <= n) {
        count += n / (idx_t)power;
        if (power > (unsigned __int128)n / p) break;
        power *= (unsigned __int128)p;
    }
    return count;
}

/* Test whether a given k satisfies binomial(k², k) ≡ 0 (mod k²). */
static bool good_k(idx_t k)
{
    idx_t temp = k;
    idx_t k2   = (idx_t)((unsigned __int128)k * (unsigned __int128)k);
    idx_t k2m = k2 - k;  /* k² – k */

    /* Iterate over prime factors of k. */
    if (temp % 2 == 0) {
        uint64_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++e;
        }
        idx_t v = v_p_factorial(k2, 2) - v_p_factorial(k, 2) -
                  v_p_factorial(k2m, 2);
        if (v < 2 * e) return false;
    }

    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            idx_t v = v_p_factorial(k2, p) - v_p_factorial(k, p) -
                      v_p_factorial(k2m, p);
            if (v < 2 * e) return false;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        uint64_t p = temp;
        idx_t v = v_p_factorial(k2, p) - v_p_factorial(k, p) -
                  v_p_factorial(k2m, p);
        if (v < 2) return false;
    }
    return true;
}

/* Reallocate the terms cache if needed. */
static void grow_cache(size_t needed, seq_t **arr, size_t *cap)
{
    if (*cap < needed) {
        size_t new_cap = (*cap) ? (*cap) << 1 : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Sequence generator. */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static idx_t next_k = 1;   /* next k to test */

    if (n == 0) return 0; /* undefined for n<1, but safety */

    while (terms_len < n) {
        grow_cache(terms_len + 1, &terms, &terms_cap);

        if (good_k(next_k))
            terms[terms_len++] = (seq_t)next_k;

        ++next_k;
        if (next_k == 0) break; /* overflow safety */
    }
    return terms[n - 1];
}
