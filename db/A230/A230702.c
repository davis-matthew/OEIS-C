/*
 * A230702  Number of (n+3)X(2+3) 0..2 white square subarrays x(i,j) with each
 *          element diagonally or antidiagonally next to at least one element
 *          with value (x(i,j)+1) mod 3, and upper left element zero.
 * https://oeis.org/A230702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31*a(n-2) - 126*a(n-4) + 42*a(n-6) + 79*a(n-8) - a(n-10)
 *            + 8*a(n-12) Initial values: a(1)=8, a(2)=28, a(3)=244, a(4)=800,
 *            a(5)=6576, a(6)=21076, a(7)=173428, a(8)=554040, a(9)=4558536,
 *            a(10)=14555468, a(11)=119758148, a(12)=382358832
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* 1‑based cache */
static size_t a_cache_cap = 0;  /* cache capacity (number of cells) */
static idx_t  a_cache_last = 0; /* highest index that has been computed */

/* Ensure cache can store indices up to N (inclusive).
   Also initialise the first 12 terms if not already done. */
static void ensure_cache(idx_t N)
{
    /* Need room for indices 0..12 at least */
    size_t needed = (size_t)N + 1;
    if (needed < 13) needed = 13;               /* indices 0..12 */
    if (a_cache == NULL)
    {
        a_cache = (seq_t*)malloc(needed * sizeof(seq_t));
        a_cache_cap = needed;
    }
    else if (a_cache_cap < needed)
    {
        a_cache = (seq_t*)realloc(a_cache,
                                  needed * sizeof(seq_t));
        a_cache_cap = needed;
    }

    /* Initialise the first 12 terms if they are not yet set */
    if (a_cache_last < 12)
    {
        static const seq_t init[12] = {
            (seq_t)8,       (seq_t)28,        (seq_t)244,
            (seq_t)800,     (seq_t)6576,      (seq_t)21076,
            (seq_t)173428,  (seq_t)554040,    (seq_t)4558536,
            (seq_t)14555468,(seq_t)119758148,(seq_t)382358832
        };
        idx_t i;
        for (i = a_cache_last + 1; i <= 12; ++i)
            a_cache[i] = init[i - 1];
        a_cache_last = 12;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, return 0 */
    ensure_cache(n);
    if (a_cache_last >= n) return a_cache[n];

    /* Compute missing values up to n */
    for (idx_t i = a_cache_last + 1; i <= n; ++i)
    {
        if (i <= 12) /* already initialised */ continue;
        seq_t pos = ((seq_t)31) * a_cache[i-2];
        pos += ((seq_t)42) * a_cache[i-6];
        pos += ((seq_t)79) * a_cache[i-8];
        pos += ((seq_t)8)  * a_cache[i-12];

        seq_t neg = ((seq_t)126) * a_cache[i-4];
        neg += a_cache[i-10];

        a_cache[i] = pos - neg;
    }
    a_cache_last = n;
    return a_cache[n];
}
