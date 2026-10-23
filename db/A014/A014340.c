/*
 * A014340  Four-fold exponential convolution of Fibonacci numbers with
 *          themselves.
 * https://oeis.org/A014340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=a(1)=a(2)=a(3)=0, a(4)=24;
   for n>=5,
   a(n) = 10*a(n-1) - 15*a(n-2) - 70*a(n-3) + 120*a(n-4) + 32*a(n-5) */

seq_t a(idx_t n) {
    if (n < 4)
        return 0;
    if (n == 4)
        return 24;

    seq_t a0 = 0;
    seq_t a1 = 0;
    seq_t a2 = 0;
    seq_t a3 = 0;
    seq_t a4 = 24;
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = 10 * a4
               - 15 * a3
               - 70 * a2
               + 120 * a1
               + 32 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
