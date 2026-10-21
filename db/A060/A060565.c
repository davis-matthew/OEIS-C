/*
 * A060565  Follow trajectory of 2n+1 in the '3x+1' problem until a lower
 *          number is reached; A060445 gives number of steps for this to
 *          happen. Sequence gives the first lower number that is reached.
 * https://oeis.org/A060565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 3390, u32 n <= 8408, u64 n <= 8408,
 *            u128 n <= 8408, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): first value lower than 2n+1 reached in the Collatz
   trajectory starting from 2n+1.  It follows the 3x+1 rules until a
   number smaller than the start appears, then returns that number. */
seq_t a(idx_t n)
{
    /* starting value 2n+1, all converted to seq_t */
    seq_t start = (seq_t)2 * (seq_t)n + (seq_t)1;
    seq_t x = start;

    /* iterate until a lower value is reached */
    while (x >= start) {
        if (x % (seq_t)2 == (seq_t)0) {
            x = x / (seq_t)2;          /* even case: x = x / 2 */
        } else {
            x = x * (seq_t)3 + (seq_t)1;/* odd case: x = 3*x + 1 */
        }
    }

    return x;  /* first value lower than the starting 2n+1 */
}
