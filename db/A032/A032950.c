/*
 * A032950  Numbers whose base-15 representation Sum_{i=0..m} d(i)*15^i has
 *          d(i)=0 for all odd i.
 * https://oeis.org/A032950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th smallest positive integer whose base‑15 *
 *            representation has zero digits at all odd positions (indices
 *            1‑based * counting from the most significant digit)
 * Range:     u8 n <= 43, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    idx_t cum = 0;                  /* accumulated count of terms so far */
    idx_t L   = 1;                  /* current length of base‑15 representation */

    /* Find length L such that a(n) has L digits. */
    while (1) {
        idx_t exp = (L - 1) / 2;    /* exponent for 15 in the block size   */
        idx_t pow = 1;              /* 15^exp                           */
        for (idx_t i = 0; i < exp; ++i) pow *= 15ULL;
        idx_t count = 14ULL * pow;  /* number of terms with length L      */

        if (cum + count >= n) break;

        cum += count;
        ++L;
    }

    /* Rank of the desired term inside its block (0‑based). */
    idx_t rank = n - cum - 1;

    /* Number of free (even‑position) digits in the block. */
    idx_t m = (L + 1) / 2;

    /* Extract the m digits (d[0] … d[m-1]) from the rank. */
    idx_t factor = 1;
    for (idx_t i = 0; i < m - 1; ++i) factor *= 15ULL;

    idx_t d[64];                    /* m ≤ 64 for any reasonable n       */
    d[0] = rank / factor + 1;       /* most significant even‑position digit */
    rank %= factor;

    for (idx_t t = 1; t < m; ++t) {
        factor /= 15ULL;
        d[t] = rank / factor;
        rank %= factor;
    }

    /* Reconstruct the decimal value from the digits. */
    seq_t value = 0;
    for (idx_t t = 0; t < m; ++t) {
        int exp = (int)(L - 1 - 2 * t);  /* exponent of 15 for this digit   */
        seq_t pow = 1;
        for (int i = 0; i < exp; ++i) pow *= 15;
        value += (seq_t)d[t] * pow;
    }

    return value;
}
