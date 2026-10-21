/*
 * A071618  a(n+1) - 3*a(n) + a(n-1) = (2/3)(1+w^(n+1)+w^(2n+2)), where w =
 *          exp(2 Pi I / 3).
 * https://oeis.org/A071618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((Lucas(2*n)-1)/2). Lucas number L_k satisfies L_k =
 *            2*F_{k-1} + F_k where F_k is the k-th Fibonacci number. The
 *            function computes a(n) using fast doubling to obtain F_{2n-1}
 *            and F_{2n} and then applies the formula. This runs in O(log n)
 *            and works for any seq_t/idx_t sizes that suppor...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast doubling: compute (F_n, F_{n+1}) */
static void fib_pair(idx_t n, seq_t *f_n, seq_t *f_n1) {
    if (n == 0) {
        *f_n  = 0;
        *f_n1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F_{k}, b = F_{k+1} for k=n/2 */
    seq_t c = a * (((b << 1) - a));    /* F_{2k}   = a*(2*b - a)      */
    seq_t d = a * a + b * b;           /* F_{2k+1} = a^2 + b^2          */
    if ((n & 1) == 0) {                /* n even: return (c,d)          */
        *f_n  = c;
        *f_n1 = d;
    } else {                           /* n odd: return (d,c+d)         */
        *f_n  = d;
        *f_n1 = c + d;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* a(0)=0 by definition */

    /* compute F_{2n-1} and F_{2n} with fast doubling */
    idx_t k = 2 * n - 1;                /* 2n-1 fits in idx_t for n>=1 */
    seq_t f_k, f_k1;                    /* f_k = F_{2n-1}, f_k1 = F_{2n} */
    fib_pair(k, &f_k, &f_k1);

    /* L_{2n} = 2*F_{2n-1} + F_{2n} */
    seq_t l = (f_k << 1) + f_k1;

    /* a(n) = floor((L_{2n}-1)/2) = (L_{2n}-1)/2 with truncation */
    return (l - 1) / 2;
}
