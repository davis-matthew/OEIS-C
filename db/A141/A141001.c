/*
 * A141001  a(n) = number of different landings of a grasshopper after n hops.
 * https://oeis.org/A141001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+1)/4) - 1 for n ≥ 9, with a(0) = 1, a(1) = 1,
 *            a(2) = 1, a(3) = 2, a(4) = 3, a(5) = 4, a(6) = 7, a(7) = 11,
 *            a(8) = 16
 * Range:     u8 n <= 15, u16 n <= 307, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First nine terms are small enough for a direct lookup. */
    static const seq_t init[9] = {
        (seq_t)1, /* a(0) */
        (seq_t)1, /* a(1) */
        (seq_t)1, /* a(2) */
        (seq_t)2, /* a(3) */
        (seq_t)3, /* a(4) */
        (seq_t)4, /* a(5) */
        (seq_t)7, /* a(6) */
        (seq_t)11,/* a(7) */
        (seq_t)16 /* a(8) */
    };

    if (n < 9) {
        return init[n];
    }

    /* Compute floor(n*(n+1)/4) - 1.  All arithmetic is in seq_t. */
    seq_t prod = (seq_t)n * (seq_t)(n + 1);   /* n*(n+1) */
    seq_t div4 = prod / 4;                    /* floor division */
    return div4 - 1;
}
