/*
 * A080004  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={-1,2}.
 * https://oeis.org/A080004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+a(n-3)+a(n-4)+4*a(n-5)-a(n-6)+a(n-7)-a(n-9)-a(n-10);
 *            Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=4, a(5)=9,
 *            a(6)=15, a(7)=25, a(8)=46, a(9)=84
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoised table of already computed terms */
    static seq_t *tbl = NULL;
    static size_t tbl_len = 0;   /* number of terms stored   */
    static size_t tbl_cap = 0;   /* allocated capacity       */

    /* convenience constants */
    static const seq_t base[10] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)4, (seq_t)9, (seq_t)15, (seq_t)25,
        (seq_t)46, (seq_t)84
    };

    /* direct return for small n */
    if (n < 10)
        return base[(size_t)n];

    /* initialise the table on first use (size 10) */
    if (tbl == NULL) {
        tbl_cap = 10;
        tbl_len  = 10;
        tbl = (seq_t *)malloc(tbl_cap * sizeof(seq_t));
        for (size_t i = 0; i < tbl_len; ++i)
            tbl[i] = base[i];
    }

    /* grow capacity if required */
    if ((size_t)n >= tbl_cap) {
        size_t new_cap = tbl_cap ? tbl_cap * 2 : 10;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *new_tbl = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < tbl_len; ++i)
            new_tbl[i] = tbl[i];
        free(tbl);
        tbl      = new_tbl;
        tbl_cap  = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = tbl_len; i <= (size_t)n; ++i) {
        seq_t a1 = tbl[i - 1];
        seq_t a3 = tbl[i - 3];
        seq_t a4 = tbl[i - 4];
        seq_t a5 = tbl[i - 5];
        seq_t a6 = tbl[i - 6];
        seq_t a7 = tbl[i - 7];
        seq_t a9 = tbl[i - 9];
        seq_t a10 = tbl[i - 10];

        /* positive part a(i-1)+a(i-3)+a(i-4)+4*a(i-5)+a(i-7) */
        seq_t pos = a1;
        pos += a3;
        pos += a4;
        pos += a5;
        pos += a5;   /* 2*a5 */
        pos += a5;   /* 3*a5 */
        pos += a5;   /* 4*a5 */
        pos += a7;

        /* negative part a(i-6)+a(i-9)+a(i-10) */
        seq_t neg = a6;
        neg += a9;
        neg += a10;

        tbl[i] = pos - neg;
    }
    tbl_len = (size_t)n + 1;
    return tbl[(size_t)n];
}
