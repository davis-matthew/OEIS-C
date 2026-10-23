/*
 * A127988  Sequence determining the parity of A025748.
 * https://oeis.org/A127988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8 * sum_{k>=0} ((n >> k) & 1) * 4^k
 * Range:     u8 n <= 7, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     = 8 * interpret the binary digits of n as a base‑4 number */

seq_t a(idx_t n)
{
    seq_t res = 0;      /* sum of the weighted bits  */
    seq_t mul = 8;      /* current weight: 8 * 4^k  */

    while (n != 0) {
        if (n & 1U)          /* bit k of n is 1 */
            res += mul;      /* add weight 8*4^k  */
        mul <<= 2;           /* next weight: multiply by 4  */
        n >>= 1;             /* move to next bit          */
    }
    return res;
}
