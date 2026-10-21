/*
 * A206641  Fibonacci sequence beginning 14, 9.
 * https://oeis.org/A206641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(1)=14, a(2)=9
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 1) return 14;
    if (n == 2) return 9;

    /* Use two variables to hold the last two computed terms.
       Note: seq_t may be a small unsigned integer type or an
       arbitrary‑precision integer.  The addition used below
       conforms to the required operators. */
    seq_t prev2 = 14;   /* a(1) */
    seq_t prev1 = 9;    /* a(2) */
    seq_t cur;

    /* Compute terms from a(3) up to a(n). */
    for (idx_t i = 3; i <= n; ++i) {
        cur   = prev1 + prev2; /* a(i) = a(i-1) + a(i-2) */
        prev2 = prev1;         /* shift previous terms */
        prev1 = cur;
    }

    return prev1; /* a(n) now stored in prev1 */
}
