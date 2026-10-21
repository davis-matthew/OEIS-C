/*
 * A054026  a(n) is the number of sets of natural numbers [a,b,c,d,e] that can
 *          be produced with the numbers [0..n] such that the values of all
 *          the distinct parenthesized expressions of a-b-c-d-e are different.
 * https://oeis.org/A054026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 3*a(n-1) - 8*a(n-3) + 6*a(n-4) + 6*a(n-5) - 8*a(n-6) +
 *            3*a(n-8) - a(n-9), a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=300,
 *            a(5)=1296, a(6)=4116, a(7)=9984, a(8)=21384
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base terms a(0)..a(8) */
    static const seq_t base[9] = {
        0, 0, 0, 0, 300,
        1296, 4116, 9984, 21384
    };
    if (n <= 8) return base[n];

    /* circular buffer holding the last 10 terms */
    seq_t buf[10];
    /* initialise with a(0)..a(8), leaving room for a(9) */
    for (idx_t i = 0; i <= 8; ++i)
        buf[i] = base[i];

    /* compute successive terms until n */
    for (idx_t i = 9; ; ++i) {
        /* compute a(i) using the recurrence */
        seq_t val = 3 * buf[(i - 1) % 10]
                    - 8 * buf[(i - 3) % 10]
                    + 6 * buf[(i - 4) % 10]
                    + 6 * buf[(i - 5) % 10]
                    - 8 * buf[(i - 6) % 10]
                    + 3 * buf[(i - 8) % 10]
                    - buf[(i - 9) % 10];

        buf[i % 10] = val;
        if (i == n) return val;
    }
}
