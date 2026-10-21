/*
 * A054421  Number of disconnected 3 X n binary matrices.
 * https://oeis.org/A054421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 52*a(n-2) + 26*a(n-3) - 35*a(n-4) for n ≥ 5;
 *            initial values: a(1)=0, a(2)=1, a(3)=11, a(4)=102
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache ------------------------------------------------------------------ */
    static seq_t *cache = NULL;          /* pointer to array of terms (1‑based) */
    static size_t cap   = 0;             /* allocated capacity (>= index of last element) */
    static size_t len   = 0;             /* number of terms stored (last index) */

    /* Helper to extend capacity to at least new_cap */
    const size_t extend_to = (cap == 0) ? 5 : (n + 1);
    if (cap < extend_to) {
        size_t new_cap = cap * 2;
        if (new_cap < extend_to) new_cap = extend_to;
        seq_t *tmp = (cap == 0) ? (seq_t*)malloc(new_cap * sizeof(seq_t))
                                 : (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 as sentinel */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize base terms if not yet done --------------------------------------- */
    if (len < 4) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)11;
        cache[4] = (seq_t)102;
        if (len < 4) len = 4;
    }

    /* Return immediately if term already computed ---------------------------------- */
    if (n <= len) return cache[n];

    /* Compute missing terms with the recurrence ----------------------------------- */
    for (size_t i = len + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1] * 14;
        seq_t t2 = cache[i-2] * 52;
        seq_t t3 = cache[i-3] * 26;
        seq_t t4 = cache[i-4] * 35;
        cache[i] = t1 - t2 + t3 - t4;
    }
    len = n;
    return cache[n];
}
