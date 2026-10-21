/*
 * A059371  a(n) = (n-1)! + ((n+1)/2)*a(n-1), a(1)=0.
 * https://oeis.org/A059371
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((3*n-1)*a(n-1) - (n-1)*n*a(n-2))/2, a(1)=0, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A059371 for n ≥ 2.  Results are memoised in a static array. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n < 2 is outside the sequence; return 0. */
    if (n < 2) return 0;

    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;        /* array indexed by n */
    static idx_t  capacity = 0;        /* number of allocated slots (≥ n+1) */
    static idx_t  computed = 0;        /* highest index that has been computed */

    /*  Allocate or enlarge the cache as needed. */
    if (capacity <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* out‑of‑memory: return 0 (failsafe) */
        cache = tmp;
        capacity = new_cap;
    }

    /*  Initialise the base cases on the first call or after a re‑allocation.  */
    if (computed < 2) {
        cache[0] = 0;   /* unused, but defined for completeness */
        cache[1] = 0;   /* a(1) */
        cache[2] = 1;   /* a(2) */
        computed = 2;   /* we have now computed indices 0‑2 */
    }

    /*  Compute values from the next missing index up to n (inclusive).  */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t t1 = (seq_t)(3 * k - 1) * cache[k - 1];
        seq_t t2 = (seq_t)(k - 1) * (seq_t)k * cache[k - 2];
        cache[k] = (t1 - t2) / 2;
    }

    computed = n;
    return cache[n];
}
