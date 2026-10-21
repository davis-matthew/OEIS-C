/*
 * A063198  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          10 ).
 * https://oeis.org/A063198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n==1; otherwise a(n) = - if n≡0 (mod3): 2*(n/3)+1 -
 *            if n≡1 (mod3): 2*((n-1)/3)-1 - if n≡2 (mod3): 2*((n-2)/3)+1 This
 *            implements the integer version of a(n) =
 *            (6*n-3+12*cos(2*n*Pi/3)-4*sqrt(3)*sin(2*n*Pi/3))/9 for n>1
 * Range:     u8 n <= 383, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    idx_t rem = n % 3;
    if (rem == 0) {
        seq_t k = (seq_t)(n / 3);
        return k * 2 + 1;
    } else if (rem == 1) {
        seq_t k = (seq_t)((n - 1) / 3);
        return k * 2 - 1;
    } else { /* rem == 2 */
        seq_t k = (seq_t)((n - 2) / 3);
        return k * 2 + 1;
    }
}
