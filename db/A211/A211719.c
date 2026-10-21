/*
 * A211719  Number of (n+1) X (n+1) -11..11 symmetric matrices with every 2 X
 *          2 subblock having sum zero.
 * https://oeis.org/A211719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11^(1+n) + 12^(1+n)
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the explicit formula a(n) = 11^(n+1) + 12^(n+1)   */
/* which holds for all n ≥ 1.                                          */

seq_t a(idx_t n)
{
    /* exponent = n + 1  (offset 1) */
    idx_t exp = n + 1;

    /* 11^(exp) */
    seq_t pow11  = (seq_t)1;
    seq_t base11 = (seq_t)11;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) pow11 *= base11;
        base11 *= base11;
        e >>= 1;
    }

    /* 12^(exp) */
    seq_t pow12  = (seq_t)1;
    seq_t base12 = (seq_t)12;
    e = exp;
    while (e > 0) {
        if (e & 1) pow12 *= base12;
        base12 *= base12;
        e >>= 1;
    }

    return pow11 + pow12;
}
