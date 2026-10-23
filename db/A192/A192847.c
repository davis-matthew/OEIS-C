/*
 * A192847  Molecular topological indices of the tetrahedral graphs.
 * https://oeis.org/A192847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)^2*(n^2-3*n+8)/4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 30, u64 n <= 690, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 1,2,3 the expression is zero because (n-3)^2 = 0 */
    if (n <= 3)
        return 0;

    /* Work in seq_t to avoid any intermediate overflow problems */
    seq_t N   = (seq_t)n;
    seq_t t   = N - 3;                     /* n-3 */
    seq_t part = N * N - 3 * N + 8;        /* n^2 - 3*n + 8 */

    /* Factors of the product */
    seq_t a = N;          /* n */
    seq_t b = N - 1;      /* n-1 */
    seq_t c = N - 2;      /* n-2 */
    seq_t d = t;          /* n-3   (first copy) */
    seq_t e = t;          /* n-3   (second copy) */
    seq_t f = part;       /* n^2-3*n+8 */

    /* One of a,b,c,d is always divisible by 4.
       Divide that factor by 4 to keep intermediate values small. */
    switch (n & 3)          /* n % 4 */
    {
        case 0: a /= 4; break;   /* n is multiple of 4 */
        case 1: b /= 4; break;   /* n-1 is multiple of 4 */
        case 2: c /= 4; break;   /* n-2 is multiple of 4 */
        case 3: d /= 4; break;   /* n-3 is multiple of 4 */
    }

    /* Multiply the remaining factors */
    seq_t result = a;
    result *= b;
    result *= c;
    result *= d;
    result *= e;
    result *= f;   /* division by 4 already performed */

    return result;
}
