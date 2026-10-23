/*
 * A022134  Fibonacci sequence beginning 4, 17.
 * https://oeis.org/A022134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2); a(0) = 4; a(1) = 17 */

struct Matrix {
    seq_t a00, a01;
    seq_t a10, a11;
};

static inline struct Matrix mat_mul(const struct Matrix *x, const struct Matrix *y)
{
    struct Matrix r;
    r.a00 = x->a00 * y->a00 + x->a01 * y->a10;
    r.a01 = x->a00 * y->a01 + x->a01 * y->a11;
    r.a10 = x->a10 * y->a00 + x->a11 * y->a10;
    r.a11 = x->a10 * y->a01 + x->a11 * y->a11;
    return r;
}

static inline struct Matrix mat_pow(struct Matrix base, idx_t exp)
{
    struct Matrix res = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };
    while (exp) {
        if (exp & (idx_t)1)
            res = mat_mul(&res, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Auxiliary matrix M = [[1,1],[1,0]] */
    struct Matrix M = { (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)0 };
    struct Matrix P = mat_pow(M, n);

    /* a(n) = P.a10 * a(1) + P.a11 * a(0) */
    return P.a10 * (seq_t)17 + P.a11 * (seq_t)4;
}
