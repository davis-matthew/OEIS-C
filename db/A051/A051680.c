/*
 * A051680  Number of n X n invertible matrices A over GF(3) such that A-I is
 *          invertible.
 * https://oeis.org/A051680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 3^{n(n-1)/2} * b(n), where b(0) = 1, b(n) = (3^n – 1) *
 *            b(n‑1) + (-1)^n. (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static idx_t last = 0;          /* last n computed (0 means none) */
    static seq_t pow3 = (seq_t)1;   /* 3^last  */
    static seq_t powC = (seq_t)1;   /* 3^{last*(last-1)/2} */
    static seq_t b    = (seq_t)1;   /* b(last)  */
    static seq_t a_res = (seq_t)0;  /* a(last)  */

    /* If the requested index is already computed, no work to do */
    if (n <= last)
        return a_res;

    /* Compute values from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* 3^i = 3^{i-1} * 3 */
        seq_t pow3_new = pow3 * (seq_t)3;

        /* 3^{i(i-1)/2} = 3^{(i-1)(i-2)/2} * 3^{i-1} */
        seq_t powC_new = powC * pow3;

        /* b(i) = (3^i – 1) * b(i-1) + (-1)^i */
        seq_t pow3_minus_1 = pow3_new - (seq_t)1;
        seq_t prod = pow3_minus_1 * b;
        seq_t b_new;
        if ((i & 1) == 0)      /* (-1)^i = +1 when i is even */
            b_new = prod + (seq_t)1;
        else                    /* (-1)^i = –1 when i is odd  */
            b_new = prod - (seq_t)1;

        /* a(i) = 3^{i(i-1)/2} * b(i) */
        a_res = powC_new * b_new;

        /* Advance state for next iteration */
        pow3 = pow3_new;
        powC = powC_new;
        b    = b_new;
    }

    last = n;
    return a_res;
}
