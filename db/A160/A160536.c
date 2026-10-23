/*
 * A160536  a(n) = Fibonacci(n) + n^2.
 * https://oeis.org/A160536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) + n^2
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void fib_pair(idx_t n, seq_t *f_n, seq_t *f_next) {
    if (n == 0) {
        *f_n = 0;
        *f_next = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);                 /* F(k), F(k+1) where k = n/2 */
    seq_t twob = ((seq_t)2 * b);              /* 2*F(k+1) */
    seq_t c = a * (twob - a);                /* F(2k)   = F(k)*(2*F(k+1)-F(k)) */
    seq_t d = a * a + b * b;                 /* F(2k+1) = F(k)^2 + F(k+1)^2 */
    if ((n & 1) == 0) {                      /* n even   */
        *f_n = c;
        *f_next = d;
    } else {                                 /* n odd    */
        *f_n = d;
        *f_next = c + d;
    }
}

seq_t a(idx_t n) {
    seq_t fibn, dummy;
    fib_pair(n, &fibn, &dummy);              /* fibn = F(n) */
    seq_t nseq = (seq_t)n;
    seq_t nsq  = nseq * nseq;                /* n^2      */
    return fibn + nsq;                       /* F(n)+n^2 */
}
