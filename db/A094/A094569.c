/*
 * A094569  Associated with alternating row sums of array in A094568.
 * https://oeis.org/A094569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (Fib(4n+5)+(-1)^n)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling algorithm for Fibonacci:
   given n, return F(n) and F(n+1) in *fn, *fn1.           */
static void fib_fast(idx_t n, seq_t *fn, seq_t *fn1) {
    /* F(0)=0, F(1)=1 */
    seq_t a = 0, b = 1;          /* a = F(k), b = F(k+1) at the start of each loop */
    const int bits = sizeof(idx_t) * 8;

    for (int i = bits - 1; i >= 0; --i) {
        /* doubling step: compute F(2k) and F(2k+1) from F(k) and F(k+1) */
        seq_t two_b_minus_a = (b << 1) - a;         /* 2*F(k+1) - F(k)  */
        seq_t c = a * two_b_minus_a;                /* F(2k)            */
        seq_t d = a * a + b * b;                    /* F(2k+1)          */
        a = c;
        b = d;
        if ((n >> i) & 1) { /* if next bit of n is 1, shift once more */
            seq_t e = a + b;  /* F(k+1)+F(k)=F(k+2) */
            a = b;
            b = e;
        }
    }
    *fn  = a; /* F(n)   */
    *fn1 = b; /* F(n+1) */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;          /* a(0) = 2   */

    /* compute Fib(4n+5) */
    idx_t k = 4 * n + 5;                  /* 4n+5 fits in idx_t for all practical n */
    seq_t f_k, f_k1;
    fib_fast(k, &f_k, &f_k1);

    /* a(n) = (Fib(4n+5) + (-1)^n) / 3
     * For even n: (+1); for odd n: (-1) => (Fib - 1)/3
     * We avoid unsigned underflow by separating cases. */
    if ((n & 1) == 0) {                   /* n even → (+1) */
        return (f_k + 1) / 3;
    } else {                              /* n odd  → (-1) */
        return (f_k - 1) / 3;
    }
}
