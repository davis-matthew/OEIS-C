/*
 * A128053  a(n)=A128056(n)/A128055(n).
 * https://oeis.org/A128053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^{n(n+1)/2} * (F(n-1)+F(n+1))*(5/6-cos(pi*n/3)/3) if
 *            n even a(n) = (-1)^{n(n+1)/2} *
 *            (F(n)+F(n+2))*(5/6-cos(pi*(n+1)/3)/3) if n odd where F(k) is the
 *            Fibonacci sequence (F(0)=0,F(1)=1)
 * Range:     i8 n <= 10, i16 n <= 20, i32 n <= 34, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    /* coefficient numerator for (5/6 - cos(pi*m/3)/3) times 6 */
    static const int coeff_numer[6] = {3, 4, 6, 7, 6, 4};

    /* sign = (-1)^{n(n+1)/2}  -> pattern +,-,-,+ for n%4 = 0,1,2,3 */
    const int sign = ((n % 4 == 1) || (n % 4 == 2)) ? -1 : 1;

    seq_t fn, fn1;            /* will hold F(k), F(k+1) for some k */
    seq_t L;                  /* Lucas number: 2*F(k+1)-F(k) */
    int num_index;            /* index into coeff_numer */
    seq_t term, result;

    if ((n & 1) == 0) {       /* n even */
        fib_pair(n, &fn, &fn1);              /* fn = F(n),  fn1 = F(n+1) */
        L = fn1 + fn1 - fn;                  /* 2*F(n+1)-F(n) = L(n) */
        num_index = (int)(n % 6);            /* m = n */
    } else {                  /* n odd */
        fib_pair(n + 1, &fn, &fn1);          /* fn = F(n+1), fn1 = F(n+2) */
        L = fn1 + fn1 - fn;                  /* 2*F(n+2)-F(n+1) = L(n+1) */
        num_index = (int)((n + 1) % 6);      /* m = n+1 */
    }

    term   = (L * coeff_numer[num_index]) / 6;
    result = term * (seq_t)sign;

    return result;
}
