/*
 * A165990  Nim sum of distinct consecutive pairs of Lucas Numbers.
 * https://oeis.org/A165990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000032(2*n) XOR A000032(2*n+1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct { seq_t a, b, c, d; } mat2;

/* matrix multiplication for 2x2 matrices of type seq_t */
static mat2 mul(mat2 x, mat2 y)
{
    mat2 r;
    r.a = x.a * y.a + x.b * y.c;
    r.b = x.a * y.b + x.b * y.d;
    r.c = x.c * y.a + x.d * y.c;
    r.d = x.c * y.b + x.d * y.d;
    return r;
}

/* fast exponentiation of the matrix M = [[1,1],[1,0]] */
static mat2 mat_pow(idx_t exp)
{
    mat2 result = {1,0,0,1};          /* identity */
    mat2 base   = {1,1,1,0};          /* M  */
    while (exp)
    {
        if (exp & 1u)
            result = mul(result, base);
        base = mul(base, base);
        exp >>= 1u;
    }
    return result;
}

/* a(n) = Lucas(2n) XOR Lucas(2n+1) */
seq_t a(idx_t n)
{
    mat2 p = mat_pow(2 * n);
    seq_t l2n  = p.c + (seq_t)2 * p.d;
    seq_t l2n1 = p.a + (seq_t)2 * p.b;
    return l2n ^ l2n1;
}
