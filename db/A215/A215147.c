/*
 * A215147  For n odd, a(n) = 1^2+2^2+3^2+...+n^2; for n even, a(n) =
 *          (1^2+2^2+3^2+...+n^2)+1.
 * https://oeis.org/A215147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((m)*(m+1)*(2*m+1))/6 + (n%2==0) where m = (n+1)/2. If n
 *            is odd, a(n) is a square pyramidal number; if n is even, a(n) is
 *            that number plus 1
 * Range:     u8 n <= 16, u16 n <= 114, u32 n <= 2881, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = floor((n+1)/2) using integer division. */
    idx_t m = (n + 1U) / 2U;
    /* Promote to seq_t for safe arithmetic. */
    seq_t mm = (seq_t)m;
    seq_t sum = mm * (mm + 1U) * (2U * mm + 1U) / 6U;
    if ((n & 1U) == 0U) {
        sum += 1U;
    }
    return sum;
}
