/*
 * A027963  T(n,n+3), T given by A027960.
 * https://oeis.org/A027963
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lucas(n+4) - (3*n^2 + 5*n + 14)/2
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling Fibonacci: computes F(n) and F(n+1) into *fn and *fn1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }

    seq_t a, b, c, d;
    fib_pair(n >> 1, &a, &b);          /* F(k), F(k+1) for k = n/2 */

    seq_t two_b = b + b;               /* 2*F(k+1) */
    seq_t temp  = two_b - a;           /* 2*F(k+1) - F(k) */

    c = a * temp;                      /* F(2k)   */
    d = a * a + b * b;                 /* F(2k+1) */

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Lucas number L(m) = 2*F(m+1) - F(m) using the above fast doubling. */
static seq_t lucas_number(idx_t m) {
    seq_t f, f_next;
    fib_pair(m, &f, &f_next);          /* f = F(m), f_next = F(m+1) */
    seq_t lucas = f_next + f_next;     /* 2*F(m+1) */
    lucas -= f;                        /* 2*F(m+1) - F(m) = L(m) */
    return lucas;
}

/* OEIS sequence A027963 */
seq_t a(idx_t n) {
    /* Compute Lucas(n+4) */
    idx_t m = n + 4;
    seq_t lucas = lucas_number(m);

    /* Compute (3*n^2 + 5*n + 14)/2 using seq_t arithmetic. */
    seq_t sn = (seq_t)n;
    seq_t term = (( (seq_t)3 * sn * sn ) + ( (seq_t)5 * sn ) + (seq_t)14 ) / 2;

    return lucas - term;
}
