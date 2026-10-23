/*
 * A033686  One-ninth of theta series of A2[hole]^2.
 * https://oeis.org/A033686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(3*n+2)/3
 * Range:     u8 n <= 45, u16 n <= 9561, u32 n <= 9561, u64 n <= 9561,
 *            u128 n <= 9561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute m = 3*n + 2 (m ≤ 2^64-1 for supported inputs) */
    idx_t m = n * 3 + 2;
    seq_t sum = 0;

    /* Sum of divisors of m */
    for (idx_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            sum += d;
            if (d != m / d) {
                sum += m / d;
            }
        }
    }

    /* Since m ≡ 2 (mod 3), sum is always divisible by 3 */
    return sum / 3;
}
