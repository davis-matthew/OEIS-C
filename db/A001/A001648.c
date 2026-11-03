/*
 * A001648  Tetranacci numbers A073817 without the leading term 4.
 * https://oeis.org/A001648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = trace of M^n, where M = [[0,1,0,0], [0,0,1,0], [0,0,0,1],
 *            [1,1,1,1]], implementing a(n) = a(n-1) + a(n-2) + a(n-3) +
 *            a(n-4)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 2997 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void mat4_mul(seq_t out[4][4], const seq_t a[4][4], const seq_t b[4][4]) {
    seq_t tmp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            seq_t sum = 0;
            for (int k = 0; k < 4; k++) {
                sum += a[i][k] * b[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out[i][j] = tmp[i][j];
        }
    }
}

static void mat4_pow(seq_t out[4][4], const seq_t M[4][4], idx_t exp) {
    seq_t result[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result[i][j] = (i == j) ? 1 : 0;
        }
    }
    
    seq_t base[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            base[i][j] = M[i][j];
        }
    }
    
    while (exp > 0) {
        if (exp & 1) {
            seq_t tmp[4][4];
            mat4_mul(tmp, result, base);
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    result[i][j] = tmp[i][j];
                }
            }
        }
        seq_t tmp[4][4];
        mat4_mul(tmp, base, base);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                base[i][j] = tmp[i][j];
            }
        }
        exp >>= 1;
    }
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out[i][j] = result[i][j];
        }
    }
}

seq_t a(idx_t n) {
    seq_t M[4][4];
    M[0][0] = 0; M[0][1] = 1; M[0][2] = 0; M[0][3] = 0;
    M[1][0] = 0; M[1][1] = 0; M[1][2] = 1; M[1][3] = 0;
    M[2][0] = 0; M[2][1] = 0; M[2][2] = 0; M[2][3] = 1;
    M[3][0] = 1; M[3][1] = 1; M[3][2] = 1; M[3][3] = 1;
    
    seq_t Mn[4][4];
    mat4_pow(Mn, M, n);
    
    return Mn[0][0] + Mn[1][1] + Mn[2][2] + Mn[3][3];
}
