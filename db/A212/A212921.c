/*
 * A212921  Composite number n = Product(p_j^k_j) that under the iteration of
 *          the map Product (p_j^k_j) -> Sum (p_j * k_j) reaches a limit that
 *          divides the number itself.
 * https://oeis.org/A212921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 6817, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   iteration_limit, sum_prime_factor_contributions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212921: composite numbers that under the iteration
   Product(p_j^k_j) → Sum(p_j*k_j) reach a fixed point that divides the
   original number.  The function returns the nth term (1‑based). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/iteration_limit.h"
#include "../utils/sum_prime_factor_contributions.h"

/* ---------- utilities for the sequence ---------- */
static bool is_composite(uint64_t m)
{
    if (m < 4) return false;               /* 1,2,3 are not composite */
    if ((m & 1) == 0) return m != 2;       /* even numbers >2 are composite */
    for (uint64_t p = 3; p * p <= m; p += 2)
        if (m % p == 0) return true;
    return false;                          /* prime */
}

/* Sum of p_j * k_j for the prime factorisation of n.             */

/* A number satisfies the property if its fixed‑point divides the
   number itself.  (Explicitly allow 4, the only composite whose
   fixed‑point equals itself.)                                 */
static bool satisfies(uint64_t n)
{
    uint64_t limit = iteration_limit(n);
    return n % limit == 0;
}

/* ---------- static cache for the sequence answers ---------- */
static seq_t *seq_cache = NULL;      /* array of sequence terms          */
static size_t seq_len   = 0;          /* how many terms have been found   */
static uint64_t next_candidate = 4;  /* next integer to test             */
#define CACHE_CAP 20000            /* big enough for n up to 10 000    */

/* Initialise the static array once.                               */
static void init_cache(void)
{
    if (seq_cache != NULL) return;
#ifdef __cplusplus
    seq_cache = new seq_t[CACHE_CAP];
#else
    seq_cache = (seq_t *)malloc(sizeof(seq_t) * CACHE_CAP);
#endif
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* not used – OEIS offset is 1 */

    init_cache();

    /* grow cache until it contains at least n terms */
    while (seq_len < n) {
        /* find next composite that satisfies the property */
        while (true) {
            if (!is_composite(next_candidate)) {
                ++next_candidate;
                continue;
            }
            if (satisfies(next_candidate))
                break;
            ++next_candidate;
        }
        /* store the valid term */
        seq_cache[seq_len++] = (seq_t)next_candidate;
        ++next_candidate;   /* next search starts after the stored value */
    }

    return seq_cache[n - 1];
}
