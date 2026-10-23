/*
 * A156230  Sum of the products of the digits of n and the positions of the
 *          digits m in n starting from the last digit.
 * https://oeis.org/A156230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Σ_{i=0}^{m-1} (i+1) * d_i where d_0 is the
 *            least‑significant digit of n * This implements the OEIS A156230
 *            definition: * a(n) = m*d1 + (m-1)*d2 + … + d_m * with d1 the
 *            rightmost digit and d_m the leftmost
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Extract decimal digits of n, least significant first */
    unsigned int digits[21];     /* max 20 decimal digits for 64‑bit n */
    int m = 0;
    unsigned long long tmp = (unsigned long long)n;
    do {
        digits[m++] = (unsigned int)(tmp % 10ULL);
        tmp /= 10ULL;
    } while (tmp);

    /* Compute weighted sum using the least‑significant position as weight 1 */
    seq_t result = 0;
    for (int i = 0; i < m; ++i) {
        result += (seq_t)(i + 1) * (seq_t)digits[i];
    }
    return result;
}
