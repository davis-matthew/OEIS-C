/*
 * A164007  Zero together with row 7 of the array in A163280.
 * https://oeis.org/A164007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+6) for n >= 7 Special early values: a(0) = 0, a(1) =
 *            13, a(2) = 26, a(3) = 33, a(4) = 52, a(5) = 55, a(6) = 78
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[7] = {0, 13, 26, 33, 52, 55, 78};

    if (n < 7)
        return init[n];

    /* For n >= 7: a(n) = n*(n+6) */
    return (seq_t)n * ((seq_t)n + 6);
}
