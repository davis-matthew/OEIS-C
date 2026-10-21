/*
 * A065173  Site swap sequence that rises infinitely after t=0. The associated
 *          delta sequence p(t)-t for the permutation of Z: A065171.
 * https://oeis.org/A065173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/2 if n even; * a(n) = (n-1)/4 if n ≡ 1 (mod 4); * a(n)
 *            = n-1 if n ≡ 3 (mod 4)
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        return (seq_t)(n / 2);
    } else if ((n & 3) == 1) {
        return (seq_t)((n - 1) / 4);
    } else {
        return (seq_t)(n - 1);
    }
}
