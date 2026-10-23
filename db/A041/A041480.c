/*
 * A041480  Numerators of continued fraction convergents to sqrt(257).
 * https://oeis.org/A041480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 32*a(n-1)+a(n-2) a(0) = 16, a(1) = 513
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

typedef struct {
    seq_t a00, a01,
          a10, a11;
} mat2x2;

static mat2x2 mat_mul(mat2x2 x, mat2x2 y)
{
    mat2x2 r;
    r.a00 = x.a00 * y.a00 + x.a01 * y.a10;
    r.a01 = x.a00 * y.a01 + x.a01 * y.a11;
    r.a10 = x.a10 * y.a00 + x.a11 * y.a10;
    r.a11 = x.a10 * y.a01 + x.a11 * y.a11;
    return r;
}

static mat2x2 mat_pow(mat2x2 base, idx_t exp)
{
    mat2x2 result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 };
    mat2x2 power = base;
    while (exp > 0)
    {
        if (exp & (idx_t)1) result = mat_mul(result, power);
        power = mat_mul(power, power);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the two initial values separately */
    if (n == (idx_t)0) return (seq_t)16;
    if (n == (idx_t)1) return (seq_t)513;

    /* For small seq_t types (≤ 64‑bit) use the iterative recurrence.
       This guarantees no intermediate overflow for values that fit in
       the target type, and stays safe for the 64‑bit build. */
    if (sizeof(seq_t) <= 8)
    {
        seq_t prev2 = (seq_t)16;   /* a(0) */
        seq_t prev1 = (seq_t)513;  /* a(1) */
        seq_t cur   = prev1;
        idx_t i;
        for (i = (idx_t)2; i <= n; ++i)
        {
            cur      = (seq_t)32 * prev1 + prev2;
            prev2 = prev1;
            prev1 = cur;
        }
        return cur;
    }

    /* For arbitrary‑precision or larger built‑ins the fast matrix
       exponentiation is used:  [a(n), a(n-1)]^T = M^(n-1) * [a(1), a(0)]^T
       where M = [[32,1],[1,0]] */
    mat2x2 base = { (seq_t)32, (seq_t)1,
                    (seq_t)1,  (seq_t)0 };
    mat2x2 M = mat_pow(base, n - (idx_t)1);

    seq_t a0 = (seq_t)16;   /* a(0) */
    seq_t a1 = (seq_t)513;  /* a(1) */

    /* a(n) = M.a00 * a(1) + M.a01 * a(0) */
    return M.a00 * a1 + M.a01 * a0;
}
