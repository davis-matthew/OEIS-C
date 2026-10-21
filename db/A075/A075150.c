/*
 * A075150  a(n) = L(n)*C(n), L(n)=Lucas numbers (A000032), C(n)=reflected
 *          Lucas numbers (see comment to A061084).
 * https://oeis.org/A075150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^n * L(2n) + 2, where L(k) are Lucas numbers. * L(k)
 *            is obtained from Fibonacci numbers via L(k)=2*F(k+1)-F(k). *
 *            Fibonacci pairs (F(n),F(n+1)) are computed by fast doubling
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 22, i64 n <= 45, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

void fib_pair(idx_t n, seq_t *f_n, seq_t *f_np1)
/* Computes F(n) and F(n+1) using fast doubling. */
{
    if (n == 0) {
        *f_n    = 0;
        *f_np1  = 1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1) where k = n/2 */

    /* c = F(2k) = F(k) * (2*F(k+1) – F(k)) */
    seq_t two_b     = b + b;            /* 2*F(k+1) */
    seq_t two_b_a   = two_b - a;        /* 2*F(k+1) – F(k) */
    seq_t c         = a * two_b_a;      /* F(2k)  */

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t a2        = a * a;            /* F(k)^2   */
    seq_t b2        = b * b;            /* F(k+1)^2 */
    seq_t d         = a2 + b2;          /* F(2k+1)  */

    if ((n & 1) == 0) {                /* n even, n=2k   */
        *f_n   = c;
        *f_np1 = d;
    } else {                           /* n odd,  n=2k+1  */
        *f_n   = d;
        *f_np1 = c + d;                /* F(2k+2) = F(2k) + F(2k+1) */
    }
}

seq_t a(idx_t n)
/* Computes the OEIS sequence A075150. */
{
    /* 2*n fits in idx_t for all practical n used here.  */
    idx_t k = n << 1;                      /* k = 2*n */

    seq_t f_k, f_kp1;
    fib_pair(k, &f_k, &f_kp1);

    /* L(2n) = 2*F(2n+1) - F(2n) */
    seq_t lucas = ((seq_t)2 * f_kp1) - f_k;

    if (n & 1) {                          /* (-1)^n factor */
        lucas = -lucas;
    }
    lucas += 2;                            /* final +2 */

    return lucas;
}
