/*
 * A094628  Erroneous version of A052218.
 * https://oeis.org/A094628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n odd ? 4 : 13) * 10^[ floor((n-1)/2) ]
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 18, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A094628, with offset 1. */

seq_t a(idx_t n) {
    if (n == 0) {
        /* Offset is 1; a(0) is undefined – return 0. */
        return 0;
    }

    /* Base multiplier: 4 for odd n, 13 for even n. */
    seq_t base = (n & 1) ? 4 : 13;

    /* Exponent of 10: floor((n-1)/2). */
    idx_t exp = (n - 1) >> 1;

    /* Compute 10^exp using 128‑bit arithmetic to avoid signed overflow. */
    unsigned __int128 power = 1;
    for (idx_t i = 0; i < exp; ++i) {
        power *= 10;
    }

    /* Final product, cast back to seq_t. */
    unsigned __int128 prod = (unsigned __int128)base * power;
    return (seq_t)prod;
}
