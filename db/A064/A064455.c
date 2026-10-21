/*
 * A064455  a(2n) = 3n, a(2n-1) = n.
 * https://oeis.org/A064455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n-1) = n, a(2n) = 3n
 * Range:     u8 n <= 171, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based, idx_t is unsigned */
    if (n & 1U)           /* odd index  -> a(n)= (n+1)/2  */
        return (seq_t)(n / 2U + 1U);
    else                  /* even index -> a(n)= 3*(n/2) */
        return (seq_t)((n / 2U) * 3U);
}
