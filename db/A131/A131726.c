/*
 * A131726  Pell numbers A000129 with 0 instead of last digit.
 * https://oeis.org/A131726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(Pell(n)/10)*10, where Pell numbers satisfy P(0)=0,
 *            P(1)=1, P(n)=2*P(n-1)+P(n-2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 2×2 matrix of seq_t values */
typedef struct {
    seq_t a00, a01, a10, a11;
} Mat;

/* Multiply two 2×2 matrices */
static Mat mat_mul(const Mat *x, const Mat *y)
{
    Mat r;
    r.a00 = x->a00 * y->a00 + x->a01 * y->a10;
    r.a01 = x->a00 * y->a01 + x->a01 * y->a11;
    r.a10 = x->a10 * y->a00 + x->a11 * y->a10;
    r.a11 = x->a10 * y->a01 + x->a11 * y->a11;
    return r;
}

/* Fast exponentiation of the Pell matrix M = [[2,1],[1,0]] */
static Mat mat_pow(idx_t exp)
{
    Mat result = {1, 0, 0, 1};     /* identity matrix */
    Mat base   = {2, 1, 1, 0};

    while (exp) {
        if (exp & 1ULL)
            result = mat_mul(&result, &base);
        base = mat_mul(&base, &base);
        exp >>= 1;
    }
    return result;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* Pell number P(n) is the (1,0) entry of M^n */
    Mat m = mat_pow(n);
    seq_t p = m.a10;                /* P(n) */
    return (p / 10) * 10;           /* replace last decimal digit with 0 */
}
