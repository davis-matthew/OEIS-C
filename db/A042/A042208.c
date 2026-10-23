/*
 * A042208  Numerators of continued fraction convergents to sqrt(630).
 * https://oeis.org/A042208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 502*a(n-2) – a(n-4) Initial values: a(0) = 25, a(1) =
 *            251, a(2) = 12575, a(3) = 126001
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The first four terms of the sequence. */
    static const seq_t init[4] = {25, 251, 12575, 126001};

    /* Return the value directly for the base cases. */
    if (n < 4) {
        return init[n];
    }

    /* Maintain the last four computed terms:
       a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1). */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];

    /* Iteratively compute up to the requested index. */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)502 * a2 - a0; /* a(i) = 502*a(i-2) – a(i-4) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3; /* a(n) is stored in a3 after the loop. */
}
