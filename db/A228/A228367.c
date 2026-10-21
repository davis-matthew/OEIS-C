/*
 * A228367  n-th element of the ruler function plus the highest power of 2
 *          dividing n.
 * https://oeis.org/A228367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = A001511(n) + A006519(n) + 1 where A001511(n) = exponent
 *            of the highest power of two dividing n and A006519(n) = the
 *            highest power of two dividing n. The sum equals v2(n) +
 *            2^{v2(n)} + 1
 * Range:     u8 n <= 255, u16 n <= 9512, u32 n <= 9512, u64 n <= 9512,
 *            u128 n <= 9512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* As the offset is 1, this function is only guaranteed for n >= 1. */
    if (n == 0) return 0;   /* safety for out‑of‑range callers */

    idx_t tmp = n;
    idx_t v2  = 0;           /* count trailing zeros of n */
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        ++v2;
    }

    seq_t power = ((seq_t)1) << v2;   /* 2^{v2}  (A006519(n)) */
    return ((seq_t)v2) + power + 1;   /* A001511(n) + A006519(n) + 1 */
}
