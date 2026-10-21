/*
 * A067987  Convolution of L(n+1) := A000204(n+1) (Lucas), n>=0, with L(n+9),
 *          n>=0.
 * https://oeis.org/A067987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (123*n+76)*F(n+1) + 76*(n+1)*F(n), where F(k) is the k-th
 *            Fibonacci number (F(0)=0, F(1)=1)
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    /* Returns fn = F(n), fn1 = F(n+1) using fast doubling. */
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    idx_t m = n >> 1;
    seq_t a, b;
    fib_pair(m, &a, &b);          // a = F(m), b = F(m+1)

    seq_t two_b = b + b;           // 2*F(m+1)
    seq_t c = a * (two_b - a);    // F(2m)
    seq_t d = a * a + b * b;      // F(2m+1)

    if ((n & 1) == 0) {           // n even
        *fn = c;
        *fn1 = d;
    } else {                      // n odd
        *fn = d;
        *fn1 = c + d;
    }
}

seq_t a(idx_t n) {
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);             // fn = F(n), fn1 = F(n+1)

    seq_t term1 = ((seq_t)(123 * n + 76)) * fn1;
    seq_t term2 = ((seq_t)(76 * (n + 1))) * fn;
    return term1 + term2;
}
