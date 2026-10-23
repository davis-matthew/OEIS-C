/*
 * A124998  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1; a(n) = trace(A^n) for n ≥ 1, where A[i][j] = 1 if
 *            |i‑j| ≤ 2, else 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void mul_matrix(seq_t out[32][32], const seq_t a[32][32], const seq_t b[32][32]) {
    for (idx_t i = 0; i < 32; ++i)
        for (idx_t j = 0; j < 32; ++j)
            out[i][j] = 0;
    for (idx_t i = 0; i < 32; ++i)
        for (idx_t k = 0; k < 32; ++k)
            if (a[i][k]) {
                for (idx_t j = 0; j < 32; ++j)
                    out[i][j] += a[i][k] * b[k][j];
            }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 1;

    static seq_t base[32][32];
    static int init = 0;
    if (!init) {
        for (idx_t i = 0; i < 32; ++i)
            for (idx_t j = 0; j < 32; ++j) {
                idx_t diff = (i >= j) ? (i - j) : (j - i);
                base[i][j] = (diff <= 2) ? (seq_t)1 : (seq_t)0;
            }
        init = 1;
    }

    seq_t result[32][32];
    seq_t pow[32][32];
    seq_t temp[32][32];

    /* initialize result = I */
    for (idx_t i = 0; i < 32; ++i)
        for (idx_t j = 0; j < 32; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* copy base to pow */
    for (idx_t i = 0; i < 32; ++i)
        for (idx_t j = 0; j < 32; ++j)
            pow[i][j] = base[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mul_matrix(temp, result, pow);
            for (idx_t i = 0; i < 32; ++i)
                for (idx_t j = 0; j < 32; ++j)
                    result[i][j] = temp[i][j];
        }
        mul_matrix(temp, pow, pow);
        for (idx_t i = 0; i < 32; ++i)
            for (idx_t j = 0; j < 32; ++j)
                pow[i][j] = temp[i][j];
        exp >>= 1;
    }

    seq_t trace = 0;
    for (idx_t i = 0; i < 32; ++i)
        trace += result[i][i];

    return trace;
}
