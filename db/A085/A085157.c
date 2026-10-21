/*
 * A085157  Quintuple factorials, 5-factorials, n!!!!!, n!5.
 * https://oeis.org/A085157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * a(n-5), with a(0) = 1 (so a(1)=1, a(2)=2, a(3)=3,
 *            a(4)=4)
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 37, u64 n <= 66, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the product n*(n-5)*(n-10)*… until the factor becomes ≤0. */
seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    idx_t t = n;
    while (t > 0) {
        prod *= (seq_t)t;
        if (t <= 5) break;   /* next subtraction would underflow */
        t -= 5;
    }
    return prod;
}
