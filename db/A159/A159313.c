/*
 * A159313  G.f.: 1/Product_{n>=1} (1 - a(n)*x^n/n!) = Sum_{n>=0}
 *          (n+1)^(n-1)*x^n/n!.
 * https://oeis.org/A159313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1) - (n-1)! * Σ_{d|n, d<n} d * (a(d)/d!)^(n/d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----- small helper: integer power by squaring --------------------------------- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* ----- dynamic storage for the sequence and factorials ------------------------ */
static seq_t *A = NULL;          /* A[n] = a(n)   for indices 0..max_n            */
static seq_t *Fact = NULL;       /* Fact[n] = n!   for indices 0..max_n           */
static idx_t  max_computed = 0;  /* highest n already computed, 0 means none   */
static size_t capA = 0;          /* current allocated size for A                */
static size_t capFact = 0;       /* current allocated size for Fact              */

/* ensure storage for at least n+1 elements (indices 0..n) */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= capA) return;

    size_t new_cap = needed * 2;
    seq_t *tmp = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = capA; i < new_cap; ++i) tmp[i] = (seq_t)0;
    A = tmp;
    capA = new_cap;

    tmp = (seq_t*)realloc(Fact, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = capFact; i < new_cap; ++i) tmp[i] = (seq_t)0;
    Fact = tmp;
    capFact = new_cap;
}

/* ----------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 for safety */

    ensure_capacity(n);

    /* initialize first entries if not already done */
    if (max_computed == 0) {
        Fact[0]   = (seq_t)1;
        A[0]      = (seq_t)0;             /* unused */
        Fact[1]   = (seq_t)1;
        A[1]      = (seq_t)1;
        max_computed = 1;
    }

    /* compute missing terms up to n */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* factorial k */
        Fact[k] = Fact[k-1] * (seq_t)k;

        /* k^(k-1) */
        seq_t term = pow_seq((seq_t)k, k-1);

        /* sum of divisor terms */
        seq_t sum_contrib = (seq_t)0;

        for (idx_t i = 1; i * i <= k; ++i) {
            if (k % i == 0) {
                idx_t d1 = i;
                idx_t d2 = k / i;

                /* process d1 */
                if (d1 < k) {
                    idx_t p = k / d1;
                    seq_t num_part = pow_seq(A[d1], p);          /* a(d)^p      */
                    seq_t den_part = pow_seq(Fact[d1], p);        /* (d!)^p      */

                    /* use 128‑bit intermediate if seq_t is 64‑bit or smaller */
#if defined(__SIZEOF_INT128__)
                    if (sizeof(seq_t) <= 8) {
                        unsigned __int128 tmp = (unsigned __int128)Fact[k-1];
                        tmp *= d1;
                        tmp *= (unsigned __int128)num_part;
                        tmp /= (unsigned __int128)den_part;
                        sum_contrib += (seq_t)tmp;
                    } else
#endif
                    {
                        seq_t tmp = Fact[k-1];
                        tmp *= d1;
                        tmp *= num_part;
                        seq_t val = tmp / den_part;
                        sum_contrib += val;
                    }
                }

                /* process d2 if distinct and proper */
                if (d2 != d1 && d2 < k) {
                    idx_t p = k / d2;
                    seq_t num_part = pow_seq(A[d2], p);
                    seq_t den_part = pow_seq(Fact[d2], p);

#if defined(__SIZEOF_INT128__)
                    if (sizeof(seq_t) <= 8) {
                        unsigned __int128 tmp = (unsigned __int128)Fact[k-1];
                        tmp *= d2;
                        tmp *= (unsigned __int128)num_part;
                        tmp /= (unsigned __int128)den_part;
                        sum_contrib += (seq_t)tmp;
                    } else
#endif
                    {
                        seq_t tmp = Fact[k-1];
                        tmp *= d2;
                        tmp *= num_part;
                        seq_t val = tmp / den_part;
                        sum_contrib += val;
                    }
                }
            }
        }
        A[k] = term - sum_contrib;
    }

    max_computed = n;
    return A[n];
}
