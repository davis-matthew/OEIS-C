/*
 * A162147  a(n) = n*(n+1)*(5*n + 4)/6.
 * https://oeis.org/A162147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(5*n + 4)/6
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t1 = nn;
    seq_t t2 = nn + (seq_t)1;
    seq_t t3 = (seq_t)5 * nn + (seq_t)4;

    /* divide by 2 */
    if ((t1 & 1) == 0)
        t1 /= 2;
    else
        t2 /= 2;

    /* divide by 3: exactly one of t1, t2, t3 is divisible by 3 */
    if ((t1 % 3) == 0)
        t1 /= 3;
    else if ((t2 % 3) == 0)
        t2 /= 3;
    else
        t3 /= 3;

    return t1 * t2 * t3;
}
