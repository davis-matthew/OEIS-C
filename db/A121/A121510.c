/*
 * A121510  Conjectured to be complete list of cycle lengths for 3x+1 (or
 *          Collatz) problem.
 * https://oeis.org/A121510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of known 3x+1 cycle lengths: 1, 2, 3, 5, 18.
   For indices beyond 5, the value is defined as 0. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    /* using a small static table for clarity; the sequence is finite. */
    static const seq_t vals[5] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)18 };
    return (n <= 5) ? vals[n - 1] : (seq_t)0;
}
