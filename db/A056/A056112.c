/*
 * A056112  a(1) = 1; a(m+1) = sum_{k=1 to m} [max(m, a(k))].
 * https://oeis.org/A056112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence:  a(1) = 1;
 *  a(n+1) = Σ_{k=1}^{n} max(n, a(k))
 *  (OEIS A056112)
 *  The implementation memoises the sequence in static arrays and
 *  grows them as needed.  It works for any seq_t / idx_t defined
 *  by the build (uint8_t .. unsigned __int128, arbitrary‑precision
 *  C++ integer, etc.).  No dynamic libraries beyond stdlib.h are used. */

#include <stdlib.h>

/* The function to be implemented */
seq_t a(idx_t n)
{
    /* static tables: seq[1..used] contains a(1)..a(used);
     * pref[i] = sum_{k=1}^{i} a(k).  Index 0 is a dummy zero.   */
    static seq_t *seq   = NULL;
    static seq_t *pref  = NULL;
    static idx_t  capacity = 0;   /* allocated size (1 + capacity elements) */
    static idx_t  used     = 0;   /* highest index computed (>=1) */

    /* ---- initialisation ------------------------------------------------ */
    if (seq == NULL) {
        capacity = 4;                /* start with capacity for indices 0..3 */
        seq  = (seq_t *)malloc(sizeof(seq_t) * capacity);
        pref = (seq_t *)malloc(sizeof(seq_t) * capacity);
        seq[0]  = (seq_t)0;
        pref[0] = (seq_t)0;
        seq[1]  = (seq_t)1;          /* a(1) = 1 */
        pref[1] = seq[1];
        used = 1;
    }

    /* ---- grow tables to be able to store a(n) --------------------------- */
    while (used < n) {
        idx_t m = used;                /* current last computed index */

        /* find j = largest index <= m with a(j) <= m  (binary search) */
        idx_t lo = 1, hi = m;
        while (lo < hi) {
            idx_t mid = (lo + hi + 1) >> 1;
            if (seq[mid] <= (seq_t)m) {
                lo = mid;
            } else {
                hi = mid - 1;
            }
        }
        idx_t j = lo;                   /* j >= 1, a(j) <= m */

        /* compute a(m+1) = m*j + (pref[m] - pref[j]) */
        seq_t part1 = (seq_t)j * (seq_t)m;                /* m*j  */
        seq_t part2 = pref[m] - pref[j];                 /* sum of a(k) > m  */
        seq_t aval  = part1 + part2;

        /* grow arrays if necessary */
        if (used + 1 >= capacity) {
            idx_t new_cap = capacity << 1;            /* double size */
            seq  = (seq_t *)realloc(seq,  sizeof(seq_t)  * new_cap);
            pref = (seq_t *)realloc(pref, sizeof(seq_t) * new_cap);
            capacity = new_cap;
        }

        /* store the new value */
        used++;
        seq[used]  = aval;
        pref[used] = pref[used - 1] + aval;
    }

    return seq[n];
}
