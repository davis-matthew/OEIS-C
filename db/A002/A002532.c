/*
 * A002532  a(n) = 2*a(n-1) + 5*a(n-2), a(0) = 0, a(1) = 1.
 * https://oeis.org/A002532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 945 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void mat_mul(seq_t out[2][2], const seq_t x[2][2], const seq_t y[2][2]) {
    seq_t temp[2][2];
    temp[0][0] = x[0][0] * y[0][0] + x[0][1] * y[1][0];
    temp[0][1] = x[0][0] * y[0][1] + x[0][1] * y[1][1];
    temp[1][0] = x[1][0] * y[0][0] + x[1][1] * y[1][0];
    temp[1][1] = x[1][0] * y[0][1] + x[1][1] * y[1][1];
    out[0][0] = temp[0][0];
    out[0][1] = temp[0][1];
    out[1][0] = temp[1][0];
    out[1][1] = temp[1][1];
}

static void mat_pow(seq_t out[2][2], const seq_t base[2][2], idx_t exp) {
    seq_t b[2][2];
    seq_t temp[2][2];
    
    b[0][0] = base[0][0]; b[0][1] = base[0][1];
    b[1][0] = base[1][0]; b[1][1] = base[1][1];
    
    out[0][0] = 1; out[0][1] = 0;
    out[1][0] = 0; out[1][1] = 1;
    
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(temp, out, b);
            out[0][0] = temp[0][0]; out[0][1] = temp[0][1];
            out[1][0] = temp[1][0]; out[1][1] = temp[1][1];
        }
        mat_mul(temp, b, b);
        b[0][0] = temp[0][0]; b[0][1] = temp[0][1];
        b[1][0] = temp[1][0]; b[1][1] = temp[1][1];
        exp >>= 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    
    seq_t M[2][2] = {{2, 5}, {1, 0}};
    seq_t result[2][2];
    mat_pow(result, M, n - 1);
    
    return result[0][0];
}
