/*
 * A030990  7-automorphic numbers ending in 3: final digits of 7n^2 agree with
 *          n.
 * https://oeis.org/A030990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the unique integer <10^n such that 7*a(n)≡1 (mod 10^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) = 3, so we initialise with that. */
    seq_t val = 3;                /* current a(k) */
    seq_t pow10 = 10;             /* 10^k current power of ten, starts at 10^1 */

    if (n <= 1)
        return val;

    for (idx_t k = 2; k <= n; ++k) {
        /* modulus = 10^k, which is pow10 * 10 */
        seq_t modulus = pow10 * 10;

        /* r = (7*val) mod 10^k */
        seq_t r = (7 * val) % modulus;

        /* s = ((r - 1) / 10^{k-1}) mod 10  --> value 0..9 */
        seq_t s = (r - 1) / pow10;

        /* t = ceil(-s / 7) mod 10, but since 7⁻¹ ≡ 3 (mod 10) we get
           t = (-s) * 3 mod 10 = (10 - (s*3%10))%10  */
        seq_t t = (10 - (s * 3) % 10) % 10;

        /* a(k) = a(k-1) + t * 10^{k-1} */
        val += t * pow10;

        /* prepare for next iteration: pow10 = 10^k */
        pow10 = modulus;
    }
    return val;
}
