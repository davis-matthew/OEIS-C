/*
 * A176326  a(3n) is the sequence 7, 6, 5, 4, ...; a(3n+1) is the sequence 9,
 *          11, 13, 15, ...; a(3n+2) is the sequence 11, 8, 5, ...
 * https://oeis.org/A176326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(3n) = 7 - n * a(3n+1) = 9 + 2n * a(3n+2) = 11 - 3n * * Thus
 *            for an arbitrary index n: * let k = n / 3; // integer division *
 *            let r = n % 3; * if r == 0: a(n) = 7 - k * if r == 1: a(n) = 9 +
 *            2k * if r == 2: a(n) = 11 - 3k
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* integer division & remainder */
    idx_t k = n / 3u;
    idx_t r = n - k * 3u;   /* same as n % 3 but avoids % on large values */

    /* use a wider type to avoid intermediate overflow */
    __int128 val = 0;

    if (r == 0u) {            /* a(3k) = 7 - k */
        val = 7 - (__int128)k;
    } else if (r == 1u) {     /* a(3k+1) = 9 + 2k */
        val = 9 + 2 * (__int128)k;
    } else {                  /* a(3k+2) = 11 - 3k */
        val = 11 - 3 * (__int128)k;
    }

    return (seq_t)val;        /* cast to the user‑supplied integer type */
}
