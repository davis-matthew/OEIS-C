/*
 * A125464  Number of base 29 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n >= 1, a(n) = trace( A^n ) where A[i][j] = 1 if
 *            |i - j| <= 8, else 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const unsigned int BASE = 29;

static void mat_identity(seq_t m[BASE][BASE]) {
    for (unsigned int i = 0; i < BASE; ++i) {
        for (unsigned int j = 0; j < BASE; ++j) {
            m[i][j] = (i == j) ? 1 : 0;
        }
    }
}

static void mat_mul(const seq_t a[BASE][BASE],
                    const seq_t b[BASE][BASE],
                    seq_t   r[BASE][BASE]) {
    seq_t tmp[BASE][BASE];
    for (unsigned int i = 0; i < BASE; ++i) {
        for (unsigned int j = 0; j < BASE; ++j) {
            seq_t sum = 0;
            for (unsigned int k = 0; k < BASE; ++k) {
                sum += a[i][k] * b[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (unsigned int i = 0; i < BASE; ++i) {
        for (unsigned int j = 0; j < BASE; ++j) {
            r[i][j] = tmp[i][j];
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;          /* a(0) = 1 */

    /* Build adjacency matrix A: A[i][j] = 1 if |i-j| <= 8 */
    seq_t A[BASE][BASE];
    for (unsigned int i = 0; i < BASE; ++i) {
        for (unsigned int j = 0; j < BASE; ++j) {
            unsigned int diff = (i >= j) ? (i - j) : (j - i);
            A[i][j] = (diff <= 8) ? 1 : 0;
        }
    }

    /* res = I (identity matrix) */
    seq_t res[BASE][BASE];
    mat_identity(res);

    /* base = A */
    seq_t base[BASE][BASE];
    for (unsigned int i = 0; i < BASE; ++i)
        for (unsigned int j = 0; j < BASE; ++j)
            base[i][j] = A[i][j];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(res, base, res);    /* res = res * base */
        }
        exp >>= 1;
        if (exp > 0) {
            mat_mul(base, base, base);  /* base = base * base */
        }
    }

    /* Compute trace of res = A^n */
    seq_t trace = 0;
    for (unsigned int i = 0; i < BASE; ++i) {
        trace += res[i][i];
    }
    return trace;
}
