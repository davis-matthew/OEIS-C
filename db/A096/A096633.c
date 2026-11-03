/*
 * A096633  Let p = n-th prime == 3 mod 8 (A007520); a(n) = smallest prime q
 *          such that p is not a square mod q.
 * https://oeis.org/A096633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined in A096633:
   Let p be the n‑th prime ≡3 (mod 8).  Return the smallest prime q
   such that p is not a quadratic residue modulo q.  For q=p (i.e.
   the Jacobi symbol is 0) we also treat p as non‑residue. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
/* provided utilities */
#include "../utils/next_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Offset 1 – nothing defined for n==0 */
    if (n == 0)
        return (seq_t)0;

    /* ------------------------------------------------------------------
       Cache of primes ≡3 (mod 8).  The cache is expanded on demand.     */
    static uint64_t *p3 = NULL;      /* array of found primes ≡3 (mod 8) */
    static size_t p3_len = 0;        /* number of stored primes          */
    static size_t p3_cap = 0;        /* allocated capacity               */
    static uint64_t last_checked = 2;/* last prime examined              */

    /* Grow until we have the n‑th desired prime */
    while (p3_len < (size_t)n)
    {
        /* Find next prime after last_checked */
        uint64_t cand = next_prime(last_checked);
        last_checked = cand;

        if (cand % 8 == 3)
        {
            /* Ensure capacity */
            if (p3_len == p3_cap)
            {
                size_t new_cap = (p3_cap == 0) ? 8 : p3_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(p3, new_cap * sizeof(uint64_t));
                if (!tmp)       /* allocation failure – return safe value */
                    return (seq_t)0;
                p3 = tmp;
                p3_cap = new_cap;
            }
            p3[p3_len++] = cand;
        }
    }

    /* ------------------------------------------------------------------
       Retrieve the desired p (n‑th prime 3 mod 8).                     */
    uint64_t p = p3[n - 1];

    /* ------------------------------------------------------------------
       Search for the smallest prime q such that p is not a square mod q.
       We treat Jacobi symbol 0 (i.e. q==p) as a non‑residue.            */
    uint64_t q = 2;
    for (;;)
    {
        /* Compute Jacobi (p/q).  For prime q this equals the Legendre symbol
           except that 0 is returned when p≡0 mod q (we treat it as non‑residue). */
        int j = jacobi_symbol(p, q);
        if (j != 1)          /* j == -1 or 0 */
            return (seq_t)q;

        /* Next prime */
        q = next_prime(q);
    }
}
