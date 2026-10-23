/*
 * A023648  Self-convolution of composite numbers.
 * https://oeis.org/A023648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{i=1}^{n} C_i · C_{n-i+1} where Cₖ is the k‑th
 *            composite number (4, 6, 8, 9, 10, 12,…)
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Helpers: static storage for primes and composites                 */
/* ------------------------------------------------------------------ */
static size_t  prime_cnt   = 0;
static size_t  prime_cap   = 0;
static uint64_t *primes    = NULL;        /* primes found so far */

static size_t  comp_cnt    = 0;
static size_t  comp_cap    = 0;
static seq_t  *comps      = NULL;         /* 0‑based: comps[0] = 4 */

static uint64_t next_candidate = 2;        /* next integer to test for primeness */

/* make sure we have at least `need` composites stored */
static void _ensure_composites(idx_t need)
{
    while (comp_cnt < need)
    {
        uint64_t cand = next_candidate;

        /* simple primality test using already found primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime)
        {
            /* add to prime list */
            if (prime_cap <= prime_cnt)
            {
                size_t new_cap = prime_cap == 0 ? 128 : prime_cap * 2;
                primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = cand;
        }
        else if (cand >= 4)   /* skip 0,1,2,3 as composites */
        {
            /* add to composite list */
            if (comp_cap <= comp_cnt)
            {
                size_t new_cap = comp_cap == 0 ? 128 : comp_cap * 2;
                comps = (seq_t *)realloc(comps, new_cap * sizeof(seq_t));
                comp_cap = new_cap;
            }
            comps[comp_cnt++] = (seq_t)cand;
        }

        ++next_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;         /* undefined for n<1 – return 0 */

    _ensure_composites(n);

    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i)
        sum += comps[i] * comps[n - 1 - i];

    return sum;
}
