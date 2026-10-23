/*
 * A172145  Smallest Fibonacci numbers with Factor 3^n.
 * https://oeis.org/A172145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = F_{4*3^{n-2}} * where F_k is the
 *            k-th Fibonacci number (F_1 = 1, F_2 = 1)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the computation via matrix exponentiation,
 * repeatedly multiplying the exponent by 3. */

#include <stdint.h>
#include <stdbool.h>

/* Forward declaration of the fast‑doubling Fibonacci function.  */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1);

/* Small matrix used for Fibonacci matrix exponentiation.
 * Represents [[a, b], [b, c]] where (a, b, c) = (F_{k+1}, F_k, F_{k-1}). */
typedef struct {
    seq_t a; /* F_{k+1} */
    seq_t b; /* F_k     */
    seq_t c; /* F_{k-1} */
} Mat;

/* Multiply two matrices of the above type. */
static void mat_mul(const Mat *x, const Mat *y, Mat *r)
{
    seq_t ax = x->a;
    seq_t bx = x->b;
    seq_t cx = x->c;
    seq_t ay = y->a;
    seq_t by = y->b;
    seq_t cy = y->c;

    r->a = ax * ay + bx * by;           /* F_{k+m+1} = a_x*a_y + b_x*b_y */
    r->b = ax * by + bx * cy;           /* F_{k+m}   = a_x*b_y + b_x*c_y */
    r->c = bx * by + cx * cy;           /* F_{k+m-1} = b_x*b_y + c_x*c_y */
}

/* Compute the Fibonacci number at position n using fast doubling. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b; /* a = F_k, b = F_{k+1} for k = n/2 */
    fib_pair(n >> 1, &a, &b);

    /* c = F_{2k} = F_k * (2*F_{k+1} - F_k) */
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);
    /* d = F_{2k+1} = F_k^2 + F_{k+1}^2 */
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Main sequence generator.  */
seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 3;       /* F_4 */

    /* Start with the matrix for k = 4  (F_4 = 3, F_5 = 5, F_3 = 2). */
    seq_t Fk, Fk1;
    fib_pair(4, &Fk, &Fk1);               /* F_4, F_5 */
    seq_t Fk_1 = Fk1 - Fk;                 /* F_3 = F_5 - F_4 */
    Mat cur = {Fk1, Fk, Fk_1};             /* M^4 */

    idx_t steps = n - 2;   /* need to multiply index by 3, step by step */
    for (idx_t i = 0; i < steps; ++i) {
        Mat sq, nxt;
        mat_mul(&cur, &cur, &sq);   /* M^{2k} */
        mat_mul(&sq,   &cur, &nxt); /* M^{3k} */
        cur = nxt;
    }
    return cur.b;   /* F_k for the final k */
}
