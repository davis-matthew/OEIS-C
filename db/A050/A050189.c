/*
 * A050189  T(n,4), array T as in A050186; a count of aperiodic binary words.
 * https://oeis.org/A050189
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for n ≥ 12:
   a(n) = 2·a(n-1) + 2·a(n-2) – 6·a(n-3) + 6·a(n-5) – 2·a(n-6) – 2·a(n-7) + a(n-8)
   Initial values:
   a(4) = 0, a(5) = 5, a(6) = 12, a(7) = 35, a(8) = 64,
   a(9) = 126, a(10) = 200, a(11) = 330
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;                          /* values below offset are irrelevant */

    /* Dynamic memoisation table */
    static seq_t *tbl = NULL;
    static idx_t cap   = 0;               /* allocated size (>= last index + 1) */
    static idx_t last  = 0;               /* highest index already computed */

    /* Extend table if necessary */
    if (n + 1 > cap) {
        idx_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(tbl, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;                     /* allocation failure → zero */
        tbl = tmp;
        cap = newcap;
    }

    /* Initial values for indices 0..11 */
    static const seq_t init[12] = {
        0, 0, 0, 0,        /* 0..3 unused */
        0, 5, 12, 35, 64, 126, 200, 330      /* 4..11 */
    };

    /* Compute new terms only if needed */
    if (n > last) {
        /* 1) fill initial terms up to index 11 */
        idx_t start = last + 1;
        if (start < 4)
            start = 4;
        if (start <= 11) {
            for (idx_t i = start; i <= n && i <= 11; ++i)
                tbl[i] = init[i];
            if (n <= 11) {
                last = n;
                return tbl[n];
            }
            last = 11;                     /* we now have values up to 11 */
        }

        /* 2) recurrence for indices > 11 */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum1 = tbl[i-1]*2 + tbl[i-2]*2 + tbl[i-5]*6 + tbl[i-8];
            seq_t sum2 = tbl[i-3]*6 + tbl[i-6]*2 + tbl[i-7]*2;
            tbl[i] = sum1 - sum2;          /* guarantee non‑negative intermediate */
        }

        last = n;
    }

    return tbl[n];
}
