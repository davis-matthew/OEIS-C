/*
 * A015733  Numbers k such that d(k) does not divide phi(k).
 * https://oeis.org/A015733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 12155, u32 n <= 18157, u64 n <= 18157,
 *            u128 n <= 18157, big any n
 * Validated: standard, pass, 73 terms through n = 9793 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th integer k (k >= 2) such that d(k) does not divide φ(k).
 * (d(k) is the number of divisors, φ(k) is Euler's totient.)
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------ */
static void phi_tau(seq_t k, seq_t *phi, seq_t *tau)
{
    seq_t n = k;
    *phi = k;          /* φ(k) starts as k */
    *tau = 1;          /* τ(k) starts at 1 */

    for (seq_t p = 2; (unsigned __int128)p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            seq_t exp = 0;
            while (n % p == 0)
            {
                n /= p;
                ++exp;
            }
            *tau *= (exp + 1);
            *phi = (*phi / p) * (p - 1);
        }
    }
    if (n > 1)          /* remaining prime factor */
    {
        *tau *= 2;
        *phi = (*phi / n) * (n - 1);
    }
}

/* ------------------------------------------------------------------ */
/* --- Helper: compute phi(k) and tau(k) (number of divisors) -------- */

/* ------------------------------------------------------------------ */
/* --- Dynamic array to cache computed terms ------------------------ */
/* ------------------------------------------------------------------ */
static seq_t *cache_seq = NULL;
static size_t   cache_len = 0;
static size_t   cache_cap = 0;
static seq_t   next_k = 2;   /* next integer to test (k >= 2) */

/* Grow cache capacity if needed */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
        seq_t *tmp = mem_resize(seq_t, cache_seq, new_cap);
        if (!tmp) abort();
        cache_seq = tmp;
        cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* --- Main sequence function ---------------------------------------- */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but safe */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        seq_t phi, tau;
        phi_tau(next_k, &phi, &tau);

        if (phi % tau != 0)          /* condition satisfied */
        {
            ensure_cache_capacity();
            cache_seq[cache_len++] = next_k;
        }
        ++next_k;   /* move to next integer */
    }

    return cache_seq[n - 1];
}
