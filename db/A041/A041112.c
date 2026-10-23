/*
 * A041112  Numerators of continued fraction convergents to sqrt(65).
 * https://oeis.org/A041112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 16*a(n-1) + a(n-2); Initial values: a(0) = 8, a(1) = 129
 *            Recurrence is applied for n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases */
    if (n == 0) {
        return (seq_t)8;
    }
    if (n == 1) {
        return (seq_t)129;
    }

    /* Use only two consecutive values of the sequence */
    seq_t prev = (seq_t)8;   /* a(n-2) */
    seq_t cur  = (seq_t)129; /* a(n-1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 16*a(i-1) + a(i-2) */
        next = (seq_t)16 * cur + prev;
        prev = cur;
        cur  = next;
    }

    return cur;
}
