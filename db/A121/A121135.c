/*
 * A121135  Numbers of isomers of unbranched a-4-catapolypentagons - see
 *          Brunvoll reference for precise definition.
 * https://oeis.org/A121135
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A121135 – Numbers of isomers of unbranched a‑4‑catopolypentagons
 *
 * The generating function is
 *   G(x) = x^3 * (16*x^10-40*x^8+8*x^7+30*x^6-18*x^4-2*x^3+12*x^2-6*x+1)
/*          -------------------------------------------- */
/*                     (2*x-1)^4 * (2*x^2-1)^2
 *
 * From the rational form one obtains the linear recurrence
 *
 *   a(n) = 8*a(n-1) - 20*a(n-2) + 76*a(n-4) - 96*a(n-5)
 *          - 32*a(n-6) + 128*a(n-7) - 64*a(n-8)      for all n ≥ 14
 *
 * Initial values (a(0)=a(1)=a(2)=0, a(3)=1):
 *
 *   a(0)=0, a(1)=0, a(2)=0, a(3)=1, a(4)=2, a(5)=8, a(6)=22, a(7)=74,
 *   a(8)=208, a(9)=598, a(10)=1600, a(11)=4248, a(12)=10880, a(13)=27512
 *
 * The function below caches results and evaluates the recurrence
 * iteratively, so that repeated calls in any order are fast.
 */

#include <stdlib.h>

/* external linkage – required by the framework */
seq_t a(idx_t n)
{
    /* minimal offset – numbers below 3 are not defined. */
    if (n < 3) return (seq_t)0;

    static seq_t *cache = NULL;      /* array of computed values */
    static size_t cap   = 0;          /* allocated capacity */
    static idx_t  size  = 0;          /* number of values stored (cache[0..size-1]) */

    /* initial values for indices 0…13 */
    static const seq_t init[14] = {
        0ULL,   /* a(0) */
        0ULL,   /* a(1) */
        0ULL,   /* a(2) */
        1ULL,   /* a(3) */
        2ULL,   /* a(4) */
        8ULL,   /* a(5) */
        22ULL,  /* a(6) */
        74ULL,  /* a(7) */
        208ULL, /* a(8) */
        598ULL, /* a(9) */
        1600ULL,/* a(10) */
        4248ULL,/* a(11) */
        10880ULL,/* a(12) */
        27512ULL/* a(13) */
    };

    /* create or enlarge the cache if necessary */
    if (size == 0) {                       /* first call – allocate */
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed – should not happen */

        /* copy the initial known terms */
        for (int i = 0; i < 14; ++i) cache[i] = init[i];
        size = 14;
    } else if ((size_t)n + 1 > cap) {      /* need more space */
        while (cap <= (size_t)n) cap <<= 1;        /* double until sufficient */
        seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* reallocation failed */
        cache = tmp;
    }

    /* extend the cache up to the requested index if not already done */
    if (n >= size) {
        for (idx_t i = size; i <= n; ++i) {
            /* values 0..13 are already initialised. The recurrence
               applies for i >= 14. */
            if (i >= 14) {
                cache[i] = cache[i-1] * (seq_t)8
                         - cache[i-2] * (seq_t)20
                         + cache[i-4] * (seq_t)76
                         - cache[i-5] * (seq_t)96
                         - cache[i-6] * (seq_t)32
                         + cache[i-7] * (seq_t)128
                         - cache[i-8] * (seq_t)64;
            }
        }
        size = n + 1;
    }

    return cache[n];
}
