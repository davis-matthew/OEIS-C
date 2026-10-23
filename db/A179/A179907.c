/*
 * A179907  Numerators in the approximation of sqrt(2) satisfying the
 *          recurrence: a(n)= [a(n-1)*a(n-2)+2]/[a(n-1)+a(n-2)] with
 *          a(1)=a(2)=1.
 * https://oeis.org/A179907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = A001333( Fibonacci(n) ) A001333(k) satisfies a1 = 1, a2 =
 *            3 and a_k = 2*a_{k-1} + a_{k-2}
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

typedef struct { seq_t a, b, c, d; } Mat;

static Mat Mat_identity(void)
{
    Mat m;
    m.a = 1; m.b = 0;
    m.c = 0; m.d = 1;
    return m;
}

static Mat Mat_power(Mat m, seq_t e)
{
    Mat result = Mat_identity();
    Mat base   = m;

    while (e > 0) {
        if ((e & 1) != 0) {
            Mat tmp;
            tmp.a = result.a * base.a + result.b * base.c;
            tmp.b = result.a * base.b + result.b * base.d;
            tmp.c = result.c * base.a + result.d * base.c;
            tmp.d = result.c * base.b + result.d * base.d;
            result = tmp;
        }
        /* square the base */
        Mat tmp;
        tmp.a = base.a * base.a + base.b * base.c;
        tmp.b = base.a * base.b + base.b * base.d;
        tmp.c = base.c * base.a + base.d * base.c;
        tmp.d = base.c * base.b + base.d * base.d;
        base = tmp;
        e >>= 1;
    }
    return result;
}

static seq_t Pell_n(seq_t k)
{
    /* A001333(k): 0th term not used, k >= 1 */
    if (k == 0) return 0;
    if (k == 1) return 1;
    if (k == 2) return 3;

    /* matrix M = [[2,1],[1,0]]   */
    Mat m;
    m.a = 2; m.b = 1;
    m.c = 1; m.d = 0;

    /* exponent = k-1   */
    seq_t exp = k - 1;
    Mat r = Mat_power(m, exp);

    /* a_k is the second component of r * [3;1]  */
    seq_t result = r.c * 3 + r.d * 1;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    seq_t f = fib(n);               /* Fib(n) as seq_t */
    return Pell_n(f);               /* a(n) = A001333(Fib(n)) */
}
