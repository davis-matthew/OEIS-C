/*
 * A021988  Decimal expansion of 1/984.
 * https://oeis.org/A021988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: digits after the decimal point in 1/984.  
   Preperiod (first 4 digits): 0, 0, 1, 0.  
   Repeating period: 1, 6, 2, 6, 0 (length 5). */

seq_t a(idx_t n) {
    static const seq_t pre[] = { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0 };
    static const seq_t cyc[] = { (seq_t)1, (seq_t)6, (seq_t)2, (seq_t)6, (seq_t)0 };

    if (n < 4)
        return pre[n];
    else
        return cyc[(n - 4) % 5];
}
