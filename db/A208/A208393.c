/*
 * A208393  Number of 2 X n 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or right-upward antidiagonal neighbors.
 * https://oeis.org/A208393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A208393: Number of 2×n 0..2 arrays with constraints.
   Recurrence: a(1)=2, a(2)=14, a(3)=117, a(4)=1017,
                a(n)=8*a(n-1)+6*a(n-2)  for n>4.
   The implementation uses fast matrix exponentiation
   (O(log n) big‑integer operations). */

typedef struct { seq_t a, b, c, d; } Matrix;

static inline Matrix mul_matrix(Matrix x, Matrix y)
{
    Matrix r;
    r.a = x.a * y.a + x.b * y.c;
    r.b = x.a * y.b + x.b * y.d;
    r.c = x.c * y.a + x.d * y.c;
    r.d = x.c * y.b + x.d * y.d;
    return r;
}

seq_t a(idx_t n)
{
    /* Handle initial values directly. */
    if (n == 1) return 2;
    if (n == 2) return 14;
    if (n == 3) return 117;
    if (n == 4) return 1017;

    /* Compute a(n) for n > 4 using matrix exponentiation. */
    idx_t k = n - 4;                     /* exponent for M^(n-4)   */
    Matrix M = {8, 6, 1, 0};             /* transition matrix      */
    Matrix R = {1, 0, 0, 1};             /* identity matrix        */

    while (k)
    {
        if (k & 1u)
            R = mul_matrix(R, M);
        M = mul_matrix(M, M);
        k >>= 1;
    }

    /* Initial vector [a(4), a(3)]^T. */
    const seq_t a4 = 1017;
    const seq_t a3 = 117;

    /* a(n) = R.a * a4 + R.b * a3. */
    seq_t result = R.a * a4 + R.b * a3;
    return result;
}
