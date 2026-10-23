/*
 * A158874  a(n) = (n + 4)*(n + 3)*(n + 2)*(n + 1)*n / 5 = 24*A000389(n+4).
 * https://oeis.org/A158874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)/5
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 114, u64 n <= 9837,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t0 = (seq_t)n;
    seq_t t1 = t0 + 1;
    seq_t t2 = t0 + 2;
    seq_t t3 = t0 + 3;
    seq_t t4 = t0 + 4;

    /* Divide one of the five terms by 5 to keep the intermediate product small */
    if (t0 % (seq_t)5 == 0) {
        t0 /= (seq_t)5;
    } else if (t1 % (seq_t)5 == 0) {
        t1 /= (seq_t)5;
    } else if (t2 % (seq_t)5 == 0) {
        t2 /= (seq_t)5;
    } else if (t3 % (seq_t)5 == 0) {
        t3 /= (seq_t)5;
    } else {  /* t4 must be divisible by 5 */
        t4 /= (seq_t)5;
    }

    return t0 * t1 * t2 * t3 * t4;
}
