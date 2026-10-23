/*
 * A047223  Numbers that are congruent to {1, 2, 3} mod 5.
 * https://oeis.org/A047223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2*floor((n-1)/3)
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term (1‑based) of OEIS A047223: numbers congruent to 1,2,3 mod 5. */
seq_t a(idx_t n)
{
    /* Offset is 1; for n==0 we provide a benign value. */
    if (n == 0)
        return 0;

    seq_t m = (seq_t)n;           /* cast index to sequence type */
    seq_t f = (m - 1) / 3;        /* floor((n-1)/3) */
    return m + 2 * f;             /* a(n) = n + 2*f */
}
