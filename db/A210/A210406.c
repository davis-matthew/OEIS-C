/*
 * A210406  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock having
 *          three or four distinct values, and new values 0..3 introduced in
 *          row major order.
 * https://oeis.org/A210406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) + 12*a(n-2), with a(1) = 7, a(2) = 78 (OEIS
 *            A210406)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the term for any n >= 1. The implementation uses only
   two temporary values and works for both C and C++ builds, regardless
   of whether seq_t is a simple integer type or an arbitrary-precision
   class. */

seq_t a(idx_t n)
{
    /* sequence is defined only for n >= 1; for n == 0 return 0 (unused) */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)7;
    if (n == 2)
        return (seq_t)78;

    /* iterate using the recurrence */
    seq_t prev2 = (seq_t)7;   /* a(1) */
    seq_t prev1 = (seq_t)78;  /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)(10) * prev1 + (seq_t)(12) * prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
