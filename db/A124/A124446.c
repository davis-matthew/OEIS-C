/*
 * A124446  a(n) = gcd(A066840(n), A124440(n)).
 * https://oeis.org/A124446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1022, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = gcd(A066840(n), A124440(n))
 *
 *  A066840(n) = sum of all integers k with 1 ≤ k ≤ ⌊n/2⌋ and gcd(k,n)=1
 *  A124440(n) = sum of all integers k with ⌊n/2⌋ < k < n  and gcd(k,n)=1
 *
 *  The implementation uses the inclusion–exclusion formula
 *  sum_{k≤L, gcd(k,n)=1} k = ∑_{d|n, d square‑free} μ(d)·d·S(⌊L/d⌋),
 *  where S(m)=m(m+1)/2.
 *
 *  The function is memoised and uses a dynamically built smallest‑prime‑factor
 *  table for efficient factorisation up to the largest n requested so far.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;          /* memoisation array for a(n) */
static bool  *cached = NULL;         /* flags whether cache[n] is valid    */
static size_t cache_size = 0;        /* current capacity of cache          */

static int  *spf = NULL;             /* smallest prime factor table        */
static size_t spf_size = 0;          /* current size of spf table          */

/* ------------------------------------------------------------------ */
/* Build or extend the spf table up to at least 'limit'.              */
static void build_spf(idx_t limit)
{
    if (limit == 0) return;

    if (spf_size >= (size_t)limit + 1)
        return;                      /* already sufficient */

    int *new_spf = (int *)realloc(spf, ((size_t)limit + 1) * sizeof(int));
    if (!new_spf) exit(1);
    spf = new_spf;

    /* initialise the new portion with the index values */
    for (size_t i = spf_size; i <= (size_t)limit; ++i)
        spf[i] = (int)i;

    /* sieve to fill spf */
    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (spf[i] != (int)i) continue;   /* not prime */
        for (size_t j = i * i; j <= (size_t)limit; j += i)
            if (spf[j] == (int)j)
                spf[j] = (int)i;
    }

    spf_size = (size_t)limit + 1;
}

/* ------------------------------------------------------------------ */
/* Compute Euler's totient φ(n) using the spf table.                 */
static seq_t phi_of(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t result = (seq_t)n;
    idx_t tmp = n;
    while (tmp > 1) {
        int p = spf[(size_t)tmp];
        while (tmp % (idx_t)p == 0)
            tmp /= (idx_t)p;
        result = result / (seq_t)p * (seq_t)(p - 1);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Compute a(n).  n ≥ 1 (offset 1).                                 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* Out of bounds; not used */

    /* Allocate or extend cache for this index */
    if (cache_size <= (size_t)n) {
        size_t newsize = (size_t)n + 1;
        seq_t *newc     = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        bool  *newcalc  = (bool  *)realloc(cached, newsize * sizeof(bool));
        if (!newc || !newcalc) exit(1);
        cache   = newc;
        cached  = newcalc;
        for (size_t i = cache_size; i < newsize; ++i)
            cached[i] = false;
        cache_size = newsize;
    }

    if (cached[(size_t)n])
        return cache[(size_t)n];

    /* Ensure factorisation data available */
    if (spf_size <= (size_t)n)
        build_spf(n);

    /* φ(n) and total sum of all coprime numbers < n */
    seq_t phi = phi_of(n);
    seq_t totalSum;
    if (n == 1)
        totalSum = 1;                /* sum of totatives of 1 */
    else
        totalSum = (seq_t)n * phi / 2;

    /* Sum of coprime numbers up to L = ⌊n/2⌋ */
    idx_t L = n / 2;
    seq_t sumSmall = (seq_t)L * (seq_t)(L + 1) / 2;   /* d = 1 term */

    if (n > 1) {
        /* collect distinct primes of n */
        uint64_t primes[20];          /* n ≤ 10000 → at most 6 distinct primes */
        size_t   pc = 0;
        idx_t tmp = n;
        while (tmp > 1) {
            int p = spf[(size_t)tmp];
            primes[pc++] = p;
            while (tmp % (idx_t)p == 0)
                tmp /= (idx_t)p;
        }

        /* inclusion–exclusion over square‑free divisors > 1 */
        size_t subsets = 1ULL << pc;
        for (size_t mask = 1; mask < subsets; ++mask) {
            uint64_t d = 1;
            size_t   bits = 0;
            for (size_t i = 0; i < pc; ++i) {
                if (mask & (1ULL << i)) {
                    d *= primes[i];
                    ++bits;
                }
            }
            idx_t m = L / (idx_t)d;
            seq_t s = (seq_t)m * (seq_t)(m + 1) / 2;
            seq_t term = d * s;
            if (bits & 1)
                sumSmall -= term;       /* μ(d) = -1 */
            else
                sumSmall += term;       /* μ(d) = +1 */
        }
    }

    seq_t sumLarge = totalSum - sumSmall;
    seq_t result   = gcd(sumSmall, sumLarge);

    cache[(size_t)n]   = result;
    cached[(size_t)n]  = true;
    return result;
}
