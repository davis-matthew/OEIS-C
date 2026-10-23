/*
 * A125416  Number of base 28 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is a 28×28 adjacency matrix * with
 *            A[i][j] = 1 if |i-j| ≤ 6 else 0, and a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t res[28][28];
    seq_t base[28][28];
    seq_t temp[28][28];

    /* res = I */
    for (int i = 0; i < 28; ++i)
        for (int j = 0; j < 28; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* base = adjacency matrix */
    for (int i = 0; i < 28; ++i)
        for (int j = 0; j < 28; ++j) {
            int d = i - j;
            if (d < 0) d = -d;
            base[i][j] = (d <= 6) ? (seq_t)1 : (seq_t)0;
        }

    idx_t e = n;
    while (e > 0) {
        if (e & 1u) {
            /* res = res * base */
            for (int i = 0; i < 28; ++i)
                for (int j = 0; j < 28; ++j) {
                    seq_t sum = 0;
                    for (int k = 0; k < 28; ++k) sum += res[i][k] * base[k][j];
                    temp[i][j] = sum;
                }
            for (int i = 0; i < 28; ++i)
                for (int j = 0; j < 28; ++j)
                    res[i][j] = temp[i][j];
        }

        /* base = base * base */
        for (int i = 0; i < 28; ++i)
            for (int j = 0; j < 28; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 28; ++k) sum += base[i][k] * base[k][j];
                temp[i][j] = sum;
            }
        for (int i = 0; i < 28; ++i)
            for (int j = 0; j < 28; ++j)
                base[i][j] = temp[i][j];

        e >>= 1;
    }

    /* trace of res */
    seq_t ans = 0;
    for (int i = 0; i < 28; ++i) ans += res[i][i];
    return ans;
}
