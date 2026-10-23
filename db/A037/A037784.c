/*
 * A037784  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,1,2,0.
 * https://oeis.org/A037784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the base‑9 number with digits (3,1,2,0,3,1,2,0,...)
 *            truncated to n digits. That is, a(n) = ∑_{k=0}^{n-1} d_k *
 *            9^{n-1-k} where d_k = perm[ k mod 4 ] and perm[0] = 3, perm[1] =
 *            1, perm[2] = 2, perm[3] = 0. Implementation: iteratively build
 *            the number from the most significant digit to the l...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t d;
        switch (i & 3u) {          /* integer mask 3 is same as %4 for non-negatives */
            case 0u: d = 3; break;
            case 1u: d = 1; break;
            case 2u: d = 2; break;
            default: d = 0; break;
        }
        res = res * 9 + d;
    }
    return res;
}
