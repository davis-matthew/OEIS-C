/*
 * A097411  Initial decimal digit of n^7.
 * https://oeis.org/A097411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = leading decimal digit of n^7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 103, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the leading decimal digit of n^7.  The value is computed
   as n^7 using integer arithmetic in type seq_t, then the most
   significant decimal digit is extracted by repeated division by 10.
   This works for all n >= 1 and for any seq_t that can hold n^7
   (for n up to about 330 000 it fits in 128‑bit).  On builds where
   seq_t is arbitrary‑precision the same operations are valid. */
seq_t a(idx_t n)
{
    if (n == 0) {          /* Offset is 1, but guard against 0. */
        return 0;
    }

    seq_t x = (seq_t)n;    /* n^1 */
    x *= (seq_t)n;         /* n^2 */
    x *= (seq_t)n;         /* n^3 */
    x *= (seq_t)n;         /* n^4 */
    x *= (seq_t)n;         /* n^5 */
    x *= (seq_t)n;         /* n^6 */
    x *= (seq_t)n;         /* n^7 */

    /* Extract the most significant decimal digit. */
    while (x >= (seq_t)10) {
        x /= (seq_t)10;
    }

    return x;   /* 0..9 */
}
