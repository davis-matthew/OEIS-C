/*
 * A188172  Number of divisors d of n of the form d == 7 (mod 8).
 * https://oeis.org/A188172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n such that d ≡ 7 (mod 8)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for index 0, safe guard */

    idx_t m = n;
    seq_t counts[8] = {0};
    counts[1] = 1;                  /* divisor 1 contributes residue 1 */

    /* Handle factor 2 separately */
    int e = 0;
    while ((m & 1ULL) == 0ULL) {    /* m % 2 == 0 */
        ++e;
        m >>= 1;
    }
    if (e > 0) {
        seq_t freq[8] = {0};
        seq_t r = 1;
        freq[1] = 1;                 /* exponent 0 */
        for (int k = 1; k <= e; ++k) {
            r = (r * 2ULL) & 7;      /* modulo 8 */
            freq[(int)r] += 1;
        }
        seq_t newc[8] = {0};
        for (int i = 0; i < 8; ++i) {
            if (!counts[i]) continue;
            for (int j = 0; j < 8; ++j) {
                if (!freq[j]) continue;
                newc[(i * j) & 7] += counts[i] * freq[j];
            }
        }
        for (int i = 0; i < 8; ++i) counts[i] = newc[i];
    }

    /* Handle odd prime factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p != 0ULL) continue;
        e = 0;
        while (m % p == 0ULL) {
            ++e;
            m /= p;
        }
        seq_t freq[8] = {0};
        seq_t r = 1;
        freq[1] = 1;                 /* exponent 0 */
        unsigned int pmod = (unsigned int)(p & 7ULL);
        for (int k = 1; k <= e; ++k) {
            r = (r * pmod) & 7;
            freq[(int)r] += 1;
        }
        seq_t newc[8] = {0};
        for (int i = 0; i < 8; ++i) {
            if (!counts[i]) continue;
            for (int j = 0; j < 8; ++j) {
                if (!freq[j]) continue;
                newc[(i * j) & 7] += counts[i] * freq[j];
            }
        }
        for (int i = 0; i < 8; ++i) counts[i] = newc[i];
    }

    /* Remaining prime factor > sqrt(original n), if any */
    if (m > 1ULL) {
        seq_t freq[8] = {0};
        freq[1] = 1;                 /* exponent 0 */
        unsigned int pmod = (unsigned int)(m & 7ULL);
        seq_t r = pmod & 7;
        freq[(int)r] += 1;           /* exponent 1 */

        seq_t newc[8] = {0};
        for (int i = 0; i < 8; ++i) {
            if (!counts[i]) continue;
            for (int j = 0; j < 8; ++j) {
                if (!freq[j]) continue;
                newc[(i * j) & 7] += counts[i] * freq[j];
            }
        }
        for (int i = 0; i < 8; ++i) counts[i] = newc[i];
    }

    return counts[7];
}
