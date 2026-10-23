/*
 * A015456  Generalized Fibonacci numbers.
 * https://oeis.org/A015456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2); a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* 2×2 matrix with elements of type seq_t:
       [a b]
       [c d] */
    struct mat2 { seq_t a, b, c, d; };

    struct mat2 result = {1, 0, 0, 1};          /* identity matrix */
    struct mat2 base   = {10, 1, 1, 0};          /* the recurrence matrix */

    idx_t exp = n - 1;
    while (exp > 0) {
        if (exp & 1) {
            /* result = result * base */
            seq_t ta = result.a * base.a + result.b * base.c;
            seq_t tb = result.a * base.b + result.b * base.d;
            seq_t tc = result.c * base.a + result.d * base.c;
            seq_t td = result.c * base.b + result.d * base.d;
            result.a = ta; result.b = tb; result.c = tc; result.d = td;
        }
        /* base = base * base */
        seq_t ta = base.a * base.a + base.b * base.c;
        seq_t tb = base.a * base.b + base.b * base.d;
        seq_t tc = base.c * base.a + base.d * base.c;
        seq_t td = base.c * base.b + base.d * base.d;
        base.a = ta; base.b = tb; base.c = tc; base.d = td;

        exp >>= 1;
    }
    /* a(n) = first row sum of the powered matrix interpreted as
       [a(n), a(n-1)]^T = M^(n-1) * [1,1]^T */
    return result.a + result.b;
}
