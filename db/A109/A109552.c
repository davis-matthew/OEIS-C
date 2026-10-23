/*
 * A109552  a(n) = floor(4076/(10-n)^2).
 * https://oeis.org/A109552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, less, dumb
 * Formula:   a(n) = floor(4076 / (9 - n)^2) (defined for 0 ≤ n ≤ 8)
 * Range:     u8 none, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n = 0 … 8.  For n > 8
       we return 0 as the values are undefined. */
    if (n > 8)
        return (seq_t)0;

    /* Compute (9 - n).  Since n ≤ 8 this subtraction is safe. */
    idx_t diff = 9 - n;

    /* (9 - n)^2 fits easily into a small integer type. */
    seq_t denom = (seq_t)diff * diff;

    /* 4076 is a compile‑time constant.  Perform integer division
       which truncates toward zero, equivalent to floor for positive
       operands. */
    return (seq_t)4076 / denom;
}
