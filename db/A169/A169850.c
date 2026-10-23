/*
 * A169850  a(n) = position of n in A169849.
 * https://oeis.org/A169850
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of n in the greedy permutation A169849 where the
 *            permutation starts with 9 and each next term is the smallest
 *            unused integer that shares at least one prime factor with the
 *            previous term
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Global arrays (lazy‑initialized) */
static idx_t    limit = 0;         /* current maximum index handled */
static uint8_t *used   = NULL;     /* used[i] == 1 if i has appeared */
static seq_t  *pos    = NULL;     /* pos[i] == position of i in the permutation */
static int    *spf    = NULL;     /* smallest prime factor for i */
static int    *next_multi = NULL; /* next unused multiple of prime p */

static void grow_limit_to(idx_t new_limit)
{
    if (new_limit <= limit)
        return;

    idx_t old_limit = limit;
    limit = new_limit;

    /* realloc with explicit casts for C++ */
    used        = (uint8_t *)realloc(used,        (size_t)(limit + 1) * sizeof(uint8_t));
    pos         = (seq_t  *)realloc(pos,         (size_t)(limit + 1) * sizeof(seq_t));
    spf         = (int    *)realloc(spf,         (size_t)(limit + 1) * sizeof(int));
    next_multi  = (int    *)realloc(next_multi,  (size_t)(limit + 1) * sizeof(int));

    /* initialise new slots */
    if (used)
        memset(used   + old_limit + 1, 0,
               (size_t)(limit - old_limit) * sizeof(uint8_t));
    if (pos)
        memset(pos    + old_limit + 1, 0,
               (size_t)(limit - old_limit) * sizeof(seq_t));

    /* initialise spf array for 2..limit */
    for (idx_t i = 2; i <= limit; ++i)
        spf[i] = (int)i;

    /* sieve for smallest prime factor */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (int)i) {
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (int)j)
                    spf[j] = (int)i;
            }
        }
    }

    /* initialise next_multi for primes */
    for (idx_t i = old_limit + 1; i <= limit; ++i) {
        if (spf[i] == (int)i)          /* i is prime */
            next_multi[i] = (int)i;
    }
}

/* factorise n into distinct primes, store in primes[8], return count in *cnt */
static void factor_distinct(idx_t n, int *primes, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        int p = spf[n];
        primes[(*cnt)++] = p;
        while (n % p == 0)
            n /= p;
    }
}

/* generate next term of the permutation,
   update last and seq_len, record position of the new term. */
static void generate_next(idx_t *last, idx_t *seq_len)
{
    int primes_arr[32];
    size_t pcnt;
    factor_distinct(*last, primes_arr, &pcnt);

    idx_t best = (idx_t)-1;
    for (size_t i = 0; i < pcnt; ++i) {
        int p = primes_arr[i];
        if (next_multi[p] == 0)
            next_multi[p] = p;           /* initialise if not set */

        /* advance to next unused multiple of p */
        while (used[(idx_t)next_multi[p]]) {
            next_multi[p] += p;
            if (next_multi[p] > limit)
                grow_limit_to(next_multi[p] + 1000);
        }
        if (best == (idx_t)-1 || next_multi[p] < best)
            best = next_multi[p];
    }

    if (best > limit)
        grow_limit_to(best + 1000);

    used[best] = 1;
    ++(*seq_len);
    if (best <= limit)
        pos[best] = (seq_t)(*seq_len);

    /* update next_multi for primes of the new term */
    factor_distinct(best, primes_arr, &pcnt);
    for (size_t i = 0; i < pcnt; ++i) {
        int p = primes_arr[i];
        while (used[(idx_t)next_multi[p]]) {
            next_multi[p] += p;
            if (next_multi[p] > limit)
                grow_limit_to(next_multi[p] + 1000);
        }
    }

    *last = best;
}

seq_t a(idx_t n) /* a(n) = position of n in the permutation */
{
    if (n < 2)
        return 0;            /* not defined for n < 2, but offset is 2 */

    static bool initialized = false;
    static idx_t  last   = 9;   /* first element of the permutation */
    static idx_t  seq_len = 1;

    if (!initialized) {
        /* initialise arrays with a modest initial size */
        limit = 200000;
        used      = (uint8_t*)malloc((size_t)(limit + 1) * sizeof(uint8_t));
        pos       = (seq_t*)malloc((size_t)(limit + 1) * sizeof(seq_t));
        spf       = (int*)malloc((size_t)(limit + 1) * sizeof(int));
        next_multi= (int*)malloc((size_t)(limit + 1) * sizeof(int));

        if (!used || !pos || !spf || !next_multi)
            return 0;        /* allocation failure */

        memset(used, 0, (size_t)(limit + 1) * sizeof(uint8_t));
        memset(pos,  0, (size_t)(limit + 1) * sizeof(seq_t));

        /* first sieve for spf */
        for (idx_t i = 2; i <= limit; ++i)
            spf[i] = (int)i;
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (spf[i] == (int)i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == (int)j)
                        spf[j] = (int)i;
            }
        }

        /* initialise next_multi for primes */
        for (idx_t i = 2; i <= limit; ++i)
            if (spf[i] == (int)i)
                next_multi[i] = (int)i;

        /* mark starting element 9 */
        used[9]   = 1;
        pos[9]    = 1;
        seq_len   = 1;
        last      = 9;

        /* skip used numbers for primes of the starting element */
        int primes[8]; size_t pcnt;
        factor_distinct(9, primes, &pcnt);
        for (size_t i = 0; i < pcnt; ++i) {
            int p = primes[i];
            while (used[(idx_t)next_multi[p]])
                next_multi[p] += p;
        }

        initialized = true;
    }

    if (n > limit)
        grow_limit_to(n + 1000);

    while (!pos[n])
        generate_next(&last, &seq_len);

    return pos[n];
}
