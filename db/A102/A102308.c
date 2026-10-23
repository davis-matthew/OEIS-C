/*
 * A102308  If n = product{primes p(k)|n} p(k)^b(n,p(k)), where p(k) is the
 *          k-th prime that divides n (when these primes are listed from
 *          smallest to largest) and each b(n,p(k)) is a positive integer,
 *          then the sequence contains the non-prime-powers n such that
 *          p(k)^b(n,p(k)) > p(k+1) for all k, 1<=k<= -1 + nu...
 * https://oeis.org/A102308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A102308 – Non‑prime‑powers with consecutive prime power dominance.
   a(n) is the n-th integer >1 such that for its prime factorisation
   n = ∏ p_k^e_k (p_k the distinct primes in increasing order),
   each p_k^e_k > p_{k+1} holds for all k = 1 … (ω(n)–1).  */

/* Standard headers.  The build supplies seq_t and idx_t. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*  Dynamic array of primes                                           */
static uint64_t *primes = NULL;           /* array of primes      */
static size_t   primes_cnt  = 0;          /* number stored       */
static size_t   primes_cap  = 0;          /* allocated capacity  */

/* extend primes up to (and including) limit */
static void extend_primes_to_limit(uint64_t limit)
{
    if (limit < 2) return;
    uint64_t start = (primes_cnt == 0) ? 2 : primes[primes_cnt-1] + 1;
    for (uint64_t cand = start; cand <= limit; ++cand)
    {
        bool is_p = true;
        for (size_t i = 0; i < primes_cnt; ++i)
        {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_p = false; break; }
        }
        if (is_p)
        {
            if (primes_cnt == primes_cap)
            {
                size_t newcap = (primes_cap == 0) ? 4 : primes_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
                if (!tmp) return;                /* allocation failure – abort silently */
                primes = tmp;
                primes_cap = newcap;
            }
            primes[primes_cnt++] = cand;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Test if integer n satisfies the inequality property.            */
static bool is_good(uint64_t n)
{
    if (n < 2) return false;

    /* factorise n with the prime list */
    uint64_t temp = n;
    uint64_t fprimes[64];
    int     fexps [64];
    size_t  fcnt   = 0;

    uint64_t limit = (uint64_t) sqrt((long double)n);
    extend_primes_to_limit(limit);
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= temp; ++i)
    {
        uint64_t p = primes[i];
        if (temp % p == 0)
        {
            int exp = 0;
            while (temp % p == 0) { temp /= p; ++exp; }
            fprimes[fcnt] = p;
            fexps [fcnt] = exp;
            ++fcnt;
        }
    }
    if (temp > 1)          /* last prime factor > sqrt(n)   */
    {
        fprimes[fcnt] = temp;
        fexps [fcnt] = 1;
        ++fcnt;
    }
    if (fcnt < 2) return false;   /* prime power or 1 */

    /* check the inequality for consecutive prime powers */
    for (size_t i = 0; i + 1 < fcnt; ++i)
    {
        uint64_t p     = fprimes[i];
        int     exp    = fexps[i];
        uint64_t nextp = fprimes[i+1];

        __int128 pow = 1;
        for (int j = 0; j < exp; ++j)
        {
            pow *= p;
            if (pow > nextp) break;      /* early exit */
        }
        if (pow <= nextp) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Sequence array                                                   */
static seq_t *seq_arr = NULL;          /* array of sequence terms   */
static size_t seq_cnt = 0;             /* number stored              */
static size_t seq_cap = 0;             /* allocated capacity         */

/* next integer to test during generation */
static uint64_t current_candidate = 2;

/* ensure that seq_arr contains at least 'needed' terms */
static void ensure_terms(size_t needed)
{
    while (seq_cnt < needed)
    {
        /* find next good number */
        while (!is_good(current_candidate)) ++current_candidate;

        /* store it */
        if (seq_cnt == seq_cap)
        {
            size_t newcap = (seq_cap == 0) ? 4 : seq_cap * 2;
            seq_t *tmp = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
            if (!tmp) return;               /* allocation failure – abort silently */
            seq_arr = tmp;
            seq_cap = newcap;
        }
        seq_arr[seq_cnt++] = (seq_t)current_candidate;
        ++current_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*  Public entry point                                               */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0 – return 0 */

    /* initialise static data on first call */
    if (!seq_arr)
    {
        seq_arr = NULL; seq_cap = 0; seq_cnt = 0;
        primes  = NULL; primes_cap = 0; primes_cnt = 0;
        current_candidate = 2;
    }

    ensure_terms((size_t)n);
    return seq_arr[n-1];
}
