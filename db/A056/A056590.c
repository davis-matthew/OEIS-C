/*
 * A056590  Fourth column sequence of triangle A056588.
 * https://oeis.org/A056590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n+4) - F(n+5)*2^(n+4) + (F(n+3)-2)*F(n+5)*F(n+4)/2
 *            where F(k) is the Fibonacci number with F(0)=0, F(1)=1
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp) {
        if (exp & 1ULL) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

/* Compute Fibonacci numbers F(n) and F(n+1) using fast doubling.
   After call: *fn  = F(n), *fn1 = F(n+1). */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a = 0; /* F(0) */
    seq_t b = 1; /* F(1) */

    /* Find the position of the highest set bit in n */
    idx_t t = n;
    int msb = 0;
    while (t >>= 1) ++msb;

    for (int i = msb; i >= 0; --i) {
        /* d = a * (2*b - a)   -> F(2k) */
        /* e = a*a + b*b       -> F(2k+1) */
        seq_t two_b_minus_a = (b << 1) - a;
        seq_t d = a * two_b_minus_a;
        seq_t e = a * a + b * b;

        if ((n >> i) & 1ULL) {
            a = e;
            b = d + e;
        } else {
            a = d;
            b = e;
        }
    }
    *fn  = a;
    *fn1 = b;
}

seq_t a(idx_t n) {
    idx_t exp = n + 4;

    seq_t pow3 = pow_seq((seq_t)3, exp);
    seq_t pow2 = pow_seq((seq_t)2, exp);

    /* Fibonacci numbers needed: F(n+3), F(n+4), F(n+5) */
    seq_t f_n3, f_n4;
    fib_pair(n + 3, &f_n3, &f_n4);     /* f_n3 = F(n+3), f_n4 = F(n+4) */
    seq_t f_n5 = f_n3 + f_n4;           /* F(n+5) = F(n+4)+F(n+3) */

    seq_t term2 = f_n5 * pow2;

    seq_t temp1 = f_n3 - (seq_t)2;
    seq_t temp2 = temp1 * f_n5;
    seq_t temp3 = temp2 * f_n4;
    seq_t term3 = temp3 / 2;

    return pow3 - term2 + term3;
}
