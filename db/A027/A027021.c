/*
 * A027021  a(n) = T(n,n) + T(n,m+1) + ... + T(n,n), where m=[ (n+2)/2 ], T
 *          given by A027011.
 * https://oeis.org/A027021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=⌊(n+2)/2⌋}^{n} T(n,k) (T is A027011). Recurrence:
 *            a(n) = 2 a(n-1) + 4 a(n-2) - 8 a(n-3) - 4 a(n-4) + 8 a(n-5) +
 *            a(n-6) - 2 a(n-7) for n ≥ 8. Initial values (n = 1 … 7): 1, 1,
 *            6, 7, 36, 56, 181
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1 – ignore n==0 */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t capacity = 0;          /* array size (number of slots) */
    static idx_t computed = 0;          /* highest index computed so far */

    /* enlarge cache if necessary */
    if (capacity <= n) {
        idx_t new_cap = (capacity ? capacity << 1 : 8);
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;        /* allocation failed */
        if (cache) {
            memcpy(new_cache, cache, capacity * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        capacity = new_cap;
    }

    /* initialise known terms once */
    if (computed == 0) {
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 6;
        cache[4] = 7;
        cache[5] = 36;
        cache[6] = 56;
        cache[7] = 181;
        computed = 7;
    }

    /* if the requested term is already computed, just return it */
    if (n <= computed) {
        return cache[n];
    }

    /* compute successive terms using the recurrence */
    for (idx_t k = computed + 1; k <= n; ++k) {
        seq_t pos = (seq_t)2 * cache[k-1]          /* 2 a(k-1) */
                + (seq_t)4 * cache[k-2]          /* +4 a(k-2) */
                + (seq_t)8 * cache[k-5]          /* +8 a(k-5) */
                +       cache[k-6];              /* + a(k-6) */

        seq_t neg = (seq_t)8 * cache[k-3]          /* 8 a(k-3) */
                + (seq_t)4 * cache[k-4]          /* +4 a(k-4) */
                + (seq_t)2 * cache[k-7];         /* +2 a(k-7) */

        cache[k] = pos - neg;                    /* final value */
    }

    computed = n;
    return cache[n];
}
