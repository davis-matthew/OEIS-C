/*
 * A030428  a(n) = 0! * 1! * 2! * ... * n! - 1.
 * https://oeis.org/A030428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = (∏_{k=0}^n k!) - 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    seq_t fact = 1;
    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0)                    /* fact = k! */
            fact *= (seq_t)k;
        prod *= fact;                  /* prod *= k! */
    }
    prod -= 1;                         /* subtract 1 */
    return prod;
}
