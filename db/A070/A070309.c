/*
 * A070309  Number of solutions 2<=x<=A060679(n) to the equation
 *          x^A060679(n)==1 (mod A060679(n)) where A060679(n) are the orders
 *          of non-cyclic groups.
 * https://oeis.org/A070309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = # { 2 <= x <= A060679(n) | x^A060679(n) ≡ 1 (mod
 *            A060679(n)) }
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   factor_u64, gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A060679(n) are the orders of non‑cyclic groups, indexed from 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Utility functions provided by the build environment */
#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/gcd.h"          /* seq_gcd */
#include "../utils/is_prime.h"     /* is_prime */

/* --------------------------------------------------------------------- */
/*  Storage for the sequence of non‑cyclic group orders                   */
/* --------------------------------------------------------------------- */
static uint64_t *m_list   = NULL;   /* orders accumulated so far      */
static size_t    m_cap    = 0;      /* current allocated capacity     */
static size_t    m_len    = 0;      /* number of stored orders        */

/* Ensure that m_list contains at least n entries (n >= 1). */
static void ensure_m_list(size_t n)
{
    while (m_len < n) {
        uint64_t cand = (m_len == 0) ? 4 : m_list[m_len - 1] + 1;
        for (;;) {
            /* Reject primes and numbers below 4. */
            if (cand < 4 || is_prime((idx_t)cand)) {
                ++cand;
                continue;
            }

            /* Factor the candidate. */
            uint64_t primes[20];
            uint64_t exps[20];
            size_t   cnt;
            factor_u64(cand, primes, exps, &cnt);

            /* Determine whether the order is of a non‑cyclic group. */
            bool noncyclic = false;

            /* If any exponent > 1, the group is guaranteed non‑cyclic. */
            for (size_t i = 0; i < cnt && !noncyclic; ++i)
                if (exps[i] > 1) noncyclic = true;

            /* Square‑free case: look for a pair of primes with a congruence. */
            if (!noncyclic) {
                for (size_t i = 0; i < cnt && !noncyclic; ++i) {
                    for (size_t j = i + 1; j < cnt && !noncyclic; ++j) {
                        uint64_t pi = primes[i], pj = primes[j];
                        if (((pj - 1) % pi) == 0 || ((pi - 1) % pj) == 0)
                            noncyclic = true;
                    }
                }
            }

            if (noncyclic) {
                /* Ensure capacity. */
                if (m_len + 1 > m_cap) {
                    size_t new_cap = m_cap ? m_cap * 2 : 64;
                    uint64_t *tmp = (uint64_t *)realloc(m_list, new_cap * sizeof(uint64_t));
                    if (!tmp) exit(1);           /* allocation failure */
                    m_list = tmp;
                    m_cap  = new_cap;
                }
                m_list[m_len++] = cand;
                break;                /* ready for next n */
            }
            ++cand;    /* try next candidate */
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Count solutions of x^m ≡ 1 (mod m) for 2 ≤ x ≤ m.                     */
/*  The count includes the identity (x ≡ 1), so we subtract 1 at the end.   */
/* --------------------------------------------------------------------- */
static seq_t count_solutions(uint64_t m)
{
    uint64_t primes[20];
    uint64_t exps[20];
    size_t   cnt;
    factor_u64(m, primes, exps, &cnt);

    seq_t res = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        if (p == 2) {
            if (e == 1) {
                /* U(2) is trivial → only the identity contributes. */
                continue;
            } else if (e == 2) {
                /* U(4) ≅ C2.  Number of solutions = gcd(m, 2). */
                seq_t g = (seq_t)(((m & 1ULL) == 0) ? 2 : 1);
                res = res * g;
            } else { /* e >= 3 */
                /* U(2^e) ≅ C2 × C_{2^{e-2}}. */
                seq_t g1 = (seq_t)(((m & 1ULL) == 0) ? 2 : 1);

                /* gcd(m, 2^{e-2}) = 2^{min(v2(m), e-2)}. */
                uint64_t tmp = m;
                uint64_t v2 = 0;
                while ((tmp & 1ULL) == 0) {
                    tmp >>= 1;
                    ++v2;
                }
                uint64_t exp = (v2 < (e - 2)) ? v2 : (e - 2);
                uint64_t g2val = 1ULL << exp;
                seq_t g2 = (seq_t)g2val;

                res = res * g1 * g2;
            }
        } else {
            /* U(p^e) is cyclic of order φ = p^(e-1)*(p-1). */
            uint64_t phi = 1;
            for (uint64_t k = 0; k < e - 1; ++k) phi *= p;
            phi *= (p - 1);

            /* Compute gcd(m, φ). */
            uint64_t a = m, b = phi;
            while (b) {
                uint64_t t = a % b;
                a = b;
                b = t;
            }
            seq_t g = (seq_t)a;
            res = res * g;
        }
    }

    /* Exclude the identity element (x ≡ 1). */
    if (res == (seq_t)0) return (seq_t)0;
    return res - (seq_t)1;
}

/* --------------------------------------------------------------------- */
/*  Public function required by the framework.                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0, but safe */

    /* Ensure the n-th non‑cyclic group order is available. */
    ensure_m_list((size_t)n);
    uint64_t m = m_list[(size_t)n - 1];

    /* Compute and return the desired number of solutions. */
    return count_solutions(m);
}
