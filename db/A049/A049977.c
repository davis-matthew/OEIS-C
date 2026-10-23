/*
 * A049977  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m =
 *          2^(p+1) + 2 - n and p is the unique integer such that 2^p < n -1
 *          <= 2^(p+1), with a(1) = 1, a(2) = 3, and a(3) = 4.
 * https://oeis.org/A049977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1}^{n-1} a(i) + a(m) for n>=4 where m = 2^(p+1) +
 *            2 - n and p is the unique integer with 2^p < n-1 <= 2^(p+1).
 *            Initial values: a(1)=1, a(2)=3, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed values so that calls
   are O(1) after the first computation up to the requested n. */

#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* for uint64_t used by idx_t, seq_t */

seq_t a(idx_t n)
{
    /* Static storage for memoisation */
    static seq_t *table  = NULL;   /* array of a(i) values, 0‑based */
    static idx_t  tcap   = 0;      /* allocated capacity */
    static idx_t  tsize  = 0;      /* number of terms currently stored */
    static seq_t  pref   = (seq_t)0; /* prefix sum of a(1)..a(tsize) */

    /* Ensure capacity for requested n */
    if (n > tcap) {
        idx_t newcap = n;
        seq_t *newtab = (seq_t *)realloc(table, newcap * sizeof(seq_t));
        if (!newtab) return (seq_t)0;  /* allocation failure, return 0 */
        table = newtab;
        tcap = newcap;
    }

    /* Compute terms up to n (if not already done) */
    for (idx_t k = tsize + 1; k <= n; ++k) {
        seq_t ak;
        if (k == 1)               ak = (seq_t)1;
        else if (k == 2)          ak = (seq_t)3;
        else if (k == 3)          ak = (seq_t)4;
        else {
            /* compute smallest power of two >= k-1 */
            idx_t pow2 = 1;
            idx_t target = k - 1;
            while (pow2 < target) pow2 <<= 1;
            /* m = 2^(p+1) + 2 - n, with 2^(p+1) == pow2 */
            idx_t m = pow2 + 2 - k;
            /* a(m) is already computed because m < k */
            ak = pref + table[m - 1];
        }
        table[k - 1] = ak;
        pref += ak;
        tsize = k;
    }

    return table[n - 1];
}
