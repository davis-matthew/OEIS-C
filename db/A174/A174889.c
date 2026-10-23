/*
 * A174889  First column of A174888.
 * https://oeis.org/A174889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if μ(n) = -1, otherwise 1. (μ is the Möbius function)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    int cnt = 0;                 /* number of distinct prime factors */

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            ++cnt;
            temp /= p;
            if (temp % p == 0)   /* square factor detected → μ(n) = 0 */
                return (seq_t)1;  /* μ(n) ≠ -1, so a(n) = 1 */
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1)                 /* remaining prime factor */
        ++cnt;

    int sign = (cnt & 1) ? -1 : 1;  /* μ(n) = sign */
    return (sign == -1) ? (seq_t)0 : (seq_t)1;
}
