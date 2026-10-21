/*
 * A092489  Arises in enumeration of 321-hexagon-avoiding permutations.
 * https://oeis.org/A092489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 9*a(n-3) - 4*a(n-4) - 4*a(n-5) +
 *            a(n-6) for n > 7 Initial values: a(1) = 0, a(2) = 0, a(3) = 1,
 *            a(4) = 4, a(5) = 14, a(6) = 48, a(7) = 165
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* First seven terms, indexed 0..6 for a(1)..a(7) */
    static const seq_t init[7] = {0, 0, 1, 4, 14, 48, 165};

    if (n <= 7)
        return init[n - 1];

    /* Sliding window holding a(n-6)..a(n-1).
       For n = 8, initialize with a(2) .. a(7) */
    seq_t c0 = init[1]; /* a(2) */
    seq_t c1 = init[2]; /* a(3) */
    seq_t c2 = init[3]; /* a(4) */
    seq_t c3 = init[4]; /* a(5) */
    seq_t c4 = init[5]; /* a(6) */
    seq_t c5 = init[6]; /* a(7) */

    seq_t new_val = 0;

    for (idx_t i = 8; i <= n; ++i) {
        new_val = (seq_t)6 * c5
                  - (seq_t)11 * c4
                  + (seq_t)9 * c3
                  - (seq_t)4 * c2
                  - (seq_t)4 * c1
                  + c0;

        /* Shift window: drop c0 (a(n-6)), shift left, append new_val as a(n) */
        c0 = c1;
        c1 = c2;
        c2 = c3;
        c3 = c4;
        c4 = c5;
        c5 = new_val;
    }

    return new_val;   /* for n <= 7 case already returned, else new_val = a(n) */
}
