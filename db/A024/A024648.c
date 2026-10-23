/*
 * A024648  n written in fractional base 8/6.
 * https://oeis.org/A024648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a( (n - (n % 8)) * 6 / 8 ) * 10 + (n % 8), with a(0) = 0,
 *            a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5, a(6) = 6, a(7)
 *            = 7
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 159, u64 n <= 2935,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence follows from the fact that in the fractional base 8/6
   the least‑significant digit is the remainder of n modulo 8, because
   d₀ must satisfy (n – d₀)·6 ≡ 0 (mod 8).  Repeating this step
   reduces n to (n – d₀)·6/8, which is smaller than n, so the
   recurrence terminates.  The value returned is the decimal integer
   whose digits are the base‑(8/6) representation of n.  */

seq_t a(idx_t n)
{
    /* Base case: all integers 0…7 have the trivial representation. */
    if (n < 8)
        return (seq_t)n;

    /* Least significant digit.  It equals n mod 8. */
    idx_t rem = n & 7;                /* n % 8 for small n */
    /* Remaining value that represents the higher positions.  */
    idx_t m = ((n - rem) / 8) * 6;    /* (n – rem) is divisible by 8 */

    /* Recursively compute the representation of the higher part. */
    seq_t high = a(m);

    /* Assemble the final decimal number: high*10 + rem.  */
    return high * 10 + (seq_t)rem;
}
