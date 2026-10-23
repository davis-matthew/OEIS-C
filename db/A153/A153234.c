/*
 * A153234  a(n) = floor(2^n/9).
 * https://oeis.org/A153234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^n / 9) (also a(n) = a(n-6) + 7*2^(n-6) for n > 5)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* first six values are trivially known */
    static const seq_t base[6] = {0, 0, 0, 0, 1, 3};
    if (n < 6)
        return base[n];

    /* Let n = k + 6*steps, with 0 <= k < 6.
       Then a(n) = a(k) + 7 * (2^k + 2^(k+6) + ... + 2^(k+6*(steps-1))).
    */
    idx_t k = n % 6;          /* 0 <= k < 6 */
    idx_t steps = (n - k) / 6;

    seq_t ans = base[k];
    seq_t pow2k = (seq_t)1 << k;   /* 2^k */

    for (idx_t i = 0; i < steps; ++i) {
        ans += (seq_t)7 * pow2k;   /* add 7*2^(k+6*i) */
        pow2k <<= 6;               /* multiply by 64 for next term */
    }
    return ans;
}
