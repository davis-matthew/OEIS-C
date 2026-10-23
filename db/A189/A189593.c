/*
 * A189593  Number of permutations of 1..n with displacements restricted to
 *          {-6,-5,-4,-3,-2,0,1}.
 * https://oeis.org/A189593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7) Initial values:
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=7, a(6)=12, a(7)=21 Offset:
 *            1 Compute lazily with memoisation. The table grows as needed
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 41, u64 n <= 82, u128 n <= 162,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *seq_cache = NULL;   /* 0‑based array, index 0 unused */
static size_t seq_cap   = 0;      /* allocated capacity (elements) */
static idx_t   seq_last  = 0;      /* highest index computed (>=0) */

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n<1; return 0 */
        return (seq_t)0;

    /* initialisation: ensure room for indices 0..7 (index 0 unused) */
    if (seq_cache == NULL) {
        seq_cap   = 8;                     /* indices 0..7 */
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        if (!seq_cache) return (seq_t)0;   /* allocation failed */

        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)1;
        seq_cache[3] = (seq_t)2;
        seq_cache[4] = (seq_t)4;
        seq_cache[5] = (seq_t)7;
        seq_cache[6] = (seq_t)12;
        seq_cache[7] = (seq_t)21;
        seq_last    = 7;
    }

    /* return cached value if already computed */
    if (n <= seq_last)
        return seq_cache[(size_t)n];

    /* Resize cache if needed */
    if ((size_t)n + 1 > seq_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_ptr = (seq_t *)realloc(seq_cache, sizeof(seq_t) * new_cap);
        if (!new_ptr) return (seq_t)0;   /* allocation failed */
        seq_cache = new_ptr;
        seq_cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        seq_t sum = seq_cache[i - 1]
                  + seq_cache[i - 3]
                  + seq_cache[i - 4]
                  + seq_cache[i - 5]
                  + seq_cache[i - 6]
                  + seq_cache[i - 7];
        seq_cache[i] = sum;
    }

    seq_last = n;
    return seq_cache[(size_t)n];
}
