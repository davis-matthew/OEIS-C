/*
 * A091074  Fibonacci sequence beginning 12, 67.
 * https://oeis.org/A091074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(1) = 12, a(2) = 67 (Fibonacci-type
 *            sequence with custom initials.)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct {
    seq_t a, b, c, d;
} mat2x2;

static mat2x2 mat_mul(const mat2x2 *x, const mat2x2 *y) {
    mat2x2 r;
    r.a = x->a * y->a + x->b * y->c;
    r.b = x->a * y->b + x->b * y->d;
    r.c = x->c * y->a + x->d * y->c;
    r.d = x->c * y->b + x->d * y->d;
    return r;
}

static mat2x2 mat_pow(mat2x2 base, idx_t exp) {
    mat2x2 result = {1, 0, 0, 1}; /* identity matrix */
    while (exp) {
        if (exp & 1)
            result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)67;
    /* Use matrix exponentiation: [a(n), a(n-1)]^T = M^(n-2) * [a(2), a(1)]^T
       where M = [[1,1],[1,0]] */
    mat2x2 M = {1, 1, 1, 0};
    mat2x2 P = mat_pow(M, n - 2);
    /* Compute a(n) = P.a * a(2) + P.b * a(1) */
    seq_t res = P.a * (seq_t)67 + P.b * (seq_t)12;
    return res;
}
