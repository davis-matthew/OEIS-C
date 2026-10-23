/*
 * A105045  a(0)=0, a(1)=1, a(2)=2, a(3)=3, a(4)=12; for n > 4, a(n) =
 *          8*a(n-2) - a(n-4) - 3.
 * https://oeis.org/A105045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-2) - a(n-4) - 3 for n > 4 Initial values: a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=12
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Memoized array to store computed values */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static idx_t capacity = 0;       /* current allocated size */
    static idx_t computed = 0;       /* highest index computed so far */

    /* First call: allocate and initialize base values */
    if (cache == NULL) {
        /* Always have room for at least indices 0..4 */
        capacity = (n < 5) ? 5 : (n + 1);
        cache = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure: return 0 */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)12;
        computed = 4;                 /* highest computed index */
    }

    /* Grow array if necessary */
    if (n + 1 > capacity) {
        idx_t newcap = n + 1;
        if (newcap < capacity * 2) newcap = capacity * 2;  /* exponential grow */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure: return 0 */
        /* Zero newly allocated entries (unnecessary but safe) */
        for (idx_t i = capacity; i < newcap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        capacity = newcap;
    }

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 4) {
            /* Base values already set */
            continue;
        }
        /* Recurrence: a(i) = 8*a(i-2) - a(i-4) - 3 */
        cache[i] = (seq_t)8 * cache[i - 2] - cache[i - 4] - (seq_t)3;
    }
    if (n > computed) computed = n;

    return cache[n];
}
