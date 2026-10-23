/*
 * A192225  Numbers that are both Fibonacci and Pell.
 * https://oeis.org/A192225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192225: Finite set of Fibonacci and Pell numbers.
   Only terms are: 0, 1, 2, 5.  The sequence terminates after the fourth element. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)5;
    /* For n > 4 the sequence is undefined; return 0 as a neutral value. */
    return (seq_t)0;
}
