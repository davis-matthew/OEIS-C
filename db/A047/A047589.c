/*
 * A047589  Numbers that are congruent to {6, 7} mod 8.
 * https://oeis.org/A047589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/2)+6 if n is odd, otherwise +7
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term (n ≥ 1) of OEIS sequence A047589.
   seq_t and idx_t are defined elsewhere in the build system. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1; undefined for 0 */
        return 0;

    idx_t q   = (n - 1) / 2;         /* floor((n-1)/2) */
    seq_t base = (seq_t)8 * (seq_t)q; /* 8*floor((n-1)/2) */

    if ((n & 1) == 0)                /* n is even */
        return base + 7;
    else                              /* n is odd */
        return base + 6;
}
