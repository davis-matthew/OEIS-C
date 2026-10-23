/*
 * A049933  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n - 1
 *          <= 2^(p+1), starting with a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A049933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049933:  a(1)=a(2)=a(3)=1;  
   for n≥4:  a(n) = a(2^(⌊log₂(n‑2)⌋+1)+2‑n) + Σ_{i=1}^{n‑1} a(i)   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for values and their prefix sums */
    static seq_t *A      = NULL;     /* a[1] … a[computed] */
    static seq_t *pref   = NULL;     /* pref[i] = Σ_{j=1}^{i} a[j] */
    static idx_t  cap    = 0;        /* allocated capacity (≥ computed+1) */
    static idx_t  computed = 0;      /* highest index whose value is known */

    if (n == 0)  /* undefined in OEIS, return 0 for safety */
        return (seq_t)0;

    /* Ensure we have room for indices up to n (1‑based) */
    if (n + 1 > cap) {
        idx_t newcap = (cap == 0) ? 4 : cap;
        if (newcap <= n) newcap = n + 1;
        while (newcap <= n) newcap <<= 1;

        A    = (seq_t*)realloc(A,   newcap * sizeof(seq_t));
        pref = (seq_t*)realloc(pref, newcap * sizeof(seq_t));
        if (!A || !pref) abort();   /* allocation error */

        /* initialise new entries to 0 */
        for (idx_t i = cap; i < newcap; ++i) {
            A[i]    = 0;
            pref[i] = 0;
        }
        cap = newcap;
    }

    /* initialise first three terms if not yet done */
    if (computed < 3) {
        A[1] = 1; pref[1] = 1;
        A[2] = 1; pref[2] = 2;
        A[3] = 1; pref[3] = 3;
        computed = 3;
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* compute floor_log2(k‑2) */
        idx_t val = k - 2;
        idx_t p = 0;
        while (val >>= 1) ++p;

        idx_t m = ((idx_t)1 << (p + 1)) + 2 - k;    /* 1‑based index, m < k */

        seq_t sumPrev = pref[k - 1];
        seq_t a_m     = A[m];

        A[k]   = sumPrev + a_m;
        pref[k] = sumPrev + A[k];
    }

    computed = n;
    return A[n];
}
