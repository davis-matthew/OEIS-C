/*
 * A187541  a(4n+2) = 2n+1, otherwise a(n) = 4n.
 * https://oeis.org/A187541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4n+2)=2n+1; otherwise a(n)=4n
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n ≡ 2 (mod 4), use the special rule. */
    if ((n & 3) == 2) {
        idx_t k = n / 4;
        /* Here k = (n-2)/4, so a(n) = 2*k + 1. */
        return ((seq_t)k) * 2 + 1;
    }
    /* Otherwise a(n) = 4*n. */
    return ((seq_t)n) * 4;
}
