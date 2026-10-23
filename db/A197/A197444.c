/*
 * A197444  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,1,1,0,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 2*a(n-1) +5*a(n-2) -4*a(n-3) -6*a(n-4) -a(n-5) +4*a(n-7) +a(n-8)  
   initial values (offset 1):  a(1)=2, a(2)=5, a(3)=12, a(4)=36, a(5)=107, a(6)=321, a(7)=957, a(8)=2868 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build system. */
static const seq_t BASE[9] = { (seq_t)0,          /* unused index 0   */
                               (seq_t)2, (seq_t)5, (seq_t)12,
                               (seq_t)36, (seq_t)107, (seq_t)321,
                               (seq_t)957, (seq_t)2868 };

/* Cache for computed values (index 1 .. n). 0‑th element is unused. */
static seq_t *CACHE = NULL;        /* pointer to array   */
static idx_t  CACHESIZE = 0;       /* allocated size     */
static idx_t  COMPUTED_MAX = 0;    /* largest index computed */

/* ------------------------------------------------------------- */
/*  a(n) – number of n×2 0..4 arrays satisfying the neighbour rule  */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Return the first 8 cached values directly */
    if (n <= 8) /* n is guaranteed >= 1 by the problem statement */
        return BASE[n];

    /* Allocate or grow the cache on the first call or when needed */
    if (!CACHE) {
        CACHESIZE = n + 1;                 /* allocate indices 0 .. n */
        CACHE = (seq_t*)malloc((size_t)CACHESIZE * sizeof(seq_t));
        if (!CACHE)
            return (seq_t)0;              /* allocation failure => 0 */
        /* initialise the first eight terms */
        for (idx_t i = 1; i <= 8; ++i)
            CACHE[i] = BASE[i];
        COMPUTED_MAX = 8;
    } else if (CACHESIZE <= n) {
        /* grow the array */
        seq_t *tmp = (seq_t*)realloc(CACHE, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;              /* allocation failure => 0 */
        CACHE = tmp;
        CACHESIZE = n + 1;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = COMPUTED_MAX + 1; i <= n; ++i) {
        seq_t v = (seq_t)2 * CACHE[i - 1];
        v += (seq_t)5 * CACHE[i - 2];
        v -= (seq_t)4 * CACHE[i - 3];
        v -= (seq_t)6 * CACHE[i - 4];
        v -=      CACHE[i - 5];
        v += (seq_t)4 * CACHE[i - 7];
        v +=      CACHE[i - 8];
        CACHE[i] = v;
    }
    COMPUTED_MAX = n;
    return CACHE[n];
}
