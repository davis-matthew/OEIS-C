/*
 * A010080  Weight distribution of [16,11,4] extended Hamming code of length
 *          16.
 * https://oeis.org/A010080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of codewords of weight 2n in the [16,11,4]
 *            extended Hamming code. It is computed from the exact weight
 *            distribution of that code
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

extern seq_t a(idx_t n)
{
    /* For weights larger than 16 there are no codewords. */
    if (n > 8)
        return 0;

    /* weight w is twice the index */
    idx_t w = n * 2;

    /* Binomial coefficients C(16, w) (pre‑computed). */
    static const seq_t C16[17] = {
        1ULL, 16ULL, 120ULL, 560ULL, 1820ULL, 4368ULL,
        8008ULL, 11440ULL, 12870ULL, 11440ULL, 8008ULL,
        4368ULL, 1820ULL, 560ULL, 120ULL, 16ULL, 1ULL
    };

    /* Binomial coefficients C(8, i) (pre‑computed). */
    static const seq_t C8[9] = {
        1ULL, 8ULL, 28ULL, 56ULL, 70ULL, 56ULL,
        28ULL, 8ULL, 1ULL
    };

    /* Compute the coefficient of z^w in (1+z)^8 (1-z)^8. */
    int64_t coeff = 0;
    for (int i = 0; i <= 8; ++i) {
        int j = (int)w - i;
        if (j < 0 || j > 8)
            continue;
        int sign = (j % 2 == 0) ? 1 : -1;
        coeff += sign * (int64_t)C8[i] * (int64_t)C8[j];
    }

    /* Number of subsets of weight w with XOR zero. */
    int64_t numerator = (int64_t)C16[w] + 15 * coeff;
    return (seq_t)(numerator / 16);
}
