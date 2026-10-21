/*
 * A077904  Expansion of (1-x)^(-1)/(1+x-2*x^3).
 * https://oeis.org/A077904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0..n} b(k) where b satisfies the linear recurrence
 *            b(k) = -b(k-1) + 2*b(k-3) with initial values b(0) = 1, b(1) =
 *            -1, b(2) = 1 The sequence a(n) is the prefix sum of b(k). This
 *            gives a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=-1, ... as in OEIS
 *            A077904
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0u) return (seq_t)1;

    /* initial b values and running sum */
    seq_t b0 = (seq_t)1;      /* b(0) */
    seq_t b1 = (seq_t)-1;     /* b(1) */
    seq_t b2 = (seq_t)1;      /* b(2) */
    seq_t sum = b0;           /* sum up to 0 */

    if (n >= 1u) sum += b1;   /* sum up to 1 */
    if (n >= 2u) sum += b2;   /* sum up to 2 */

    /* iterate from k = 3 to n */
    for (idx_t k = 3u; k <= n; ++k) {
        seq_t bn = -b2 + (seq_t)2 * b0;  /* b(k) = -b(k-1) + 2*b(k-3) */
        sum += bn;

        /* shift the window: b0<-b1, b1<-b2, b2<-bn */
        b0 = b1;
        b1 = b2;
        b2 = bn;
    }

    return sum;
}
