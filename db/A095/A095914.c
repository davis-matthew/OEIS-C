/*
 * A095914  Binary numbers with 2 replacing 1 in odd positions.
 * https://oeis.org/A095914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{i≥0} ( ((n >> i) & 1) * (i%2==0 ? 2 : 1) * 10^i )
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Binary numbers with 2 replacing 1 in odd positions.  */
seq_t a(idx_t n)
{
    /* Fast iterate over the bits of n. */
    seq_t result  = 0;   /* accumulating value                     */
    seq_t pow10   = 1;   /* 10^i as we walk through positions      */
    idx_t  i      = 0;   /* current bit index (0 = least‑significant) */

    /* Handle the trivial case immediately. */
    if (n == 0)
        return 0;

    for (; n != 0; n >>= 1, ++i, pow10 *= 10)
    {
        if (n & 1)               /* bit 1 present at position i     */
        {
            seq_t digit = (i & 1) ? 1 : 2;  /* odd i (1‑based) -> 2, else 1 */
            result += digit * pow10;
        }
    }

    return result;
}
