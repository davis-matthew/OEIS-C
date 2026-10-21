/*
 * A211572  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having one,
 *          two, three, four or five distinct values for every i,j,k<=n.
 * https://oeis.org/A211572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2) - 46*a(n-3) + 172*a(n-4) + 6*a(n-5)
 *            - 626*a(n-6) + 470*a(n-7) + 1009*a(n-8) - 1253*a(n-9) -
 *            709*a(n-10) + 1354*a(n-11) + 140*a(n-12) - 663*a(n-13) +
 *            23*a(n-14) + 148*a(n-15) - 6*a(n-16) - 12*a(n-17). Initial
 *            values: a(1)=25, a(2)=69, a(3)=177, a(4)=427, a(5)=1009, a(6)...
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/*  The maximal n we expect to handle.  Adjust if necessary. */
#define A211572_MAX_N 20001

/* Static cache of computed terms.  Index i holds a(i+1). */
static seq_t a_cache[A211572_MAX_N];

/* Number of terms currently cached (always >= 0). */
static size_t a_cache_len = 0;

/* Initial values directly in the type seq_t.  The cast ensures
   compatibility for all seq_t sizes. */
static const seq_t init_vals[17] = {
    (seq_t)25ULL,    (seq_t)69ULL,    (seq_t)177ULL,   (seq_t)427ULL,
    (seq_t)1009ULL,  (seq_t)2339ULL,  (seq_t)5381ULL,   (seq_t)12315ULL,
    (seq_t)28147ULL, (seq_t)64367ULL, (seq_t)147473ULL, (seq_t)338895ULL,
    (seq_t)781407ULL, (seq_t)1808731ULL, (seq_t)4202661ULL,
    (seq_t)9803439ULL, (seq_t)22952115ULL
};

/* Compute the sequence up to index n (1‑based).  The function is
   thread‑safe for incremental calls and works in any call order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure the internal cache is large enough.  We keep a static
       maximum array, so an argument larger than the maximum results
       in an out‑of‑range access, which is undefined behaviour but
       the sequence is not required beyond A211572_MAX_N-1. */
    if ((size_t)n > A211572_MAX_N - 1)
        return (seq_t)0;   /* or handle as desired */

    /* Compute missing terms in increasing order. */
    for (size_t i = a_cache_len; i < (size_t)n; ++i) {
        seq_t val;
        if (i < 17) {
            val = init_vals[i];
        } else {
            /* Use the recurrence.  All arithmetic uses seq_t types
               so it works for both native integer and arbitrary‑precision
               builds. */
            val = 8 * a_cache[i - 1] - 14 * a_cache[i - 2]
                - 46 * a_cache[i - 3] + 172 * a_cache[i - 4]
                + 6 * a_cache[i - 5] - 626 * a_cache[i - 6]
                + 470 * a_cache[i - 7] + 1009 * a_cache[i - 8]
                - 1253 * a_cache[i - 9] - 709 * a_cache[i -10]
                + 1354 * a_cache[i -11] + 140 * a_cache[i -12]
                - 663 * a_cache[i -13] + 23 * a_cache[i -14]
                + 148 * a_cache[i -15] - 6 * a_cache[i -16]
                - 12 * a_cache[i -17];
        }
        a_cache[i] = val;
    }
    a_cache_len = (size_t)n;

    return a_cache[n - 1];
}
