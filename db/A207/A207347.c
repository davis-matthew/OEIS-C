/*
 * A207347  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 0 and 1 1 1 vertically.
 * https://oeis.org/A207347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) - 2*a(n-4) - a(n-5) - a(n-6) for n > 7
 *            with initial values a(1)=6, a(2)=36, a(3)=84, a(4)=192,
 *            a(5)=426, a(6)=858, a(7)=1704
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 30, u64 n <= 67, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For the sequence A207347 */
#include <stddef.h>          /* size_t alias for the modulus index */

static const seq_t init[8] = {
    0,          /* unused at index 0 */
    6,          /* a(1) */
    36,         /* a(2) */
    84,         /* a(3) */
    192,        /* a(4) */
    426,        /* a(5) */
    858,        /* a(6) */
    1704        /* a(7) */
};

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* out‑of‑range guard, not required */

    /* small fast paths */
    if (n <= 7) return init[(size_t)n];

    /* store the last seven values in a circular buffer */
    seq_t buf[7];
    for (size_t i = 1; i <= 7; ++i)
        buf[i % 7] = init[(size_t)i];

    size_t m = (size_t)n;
    for (size_t i = 8; i <= m; ++i) {
        seq_t v = 2 * buf[(i - 1) % 7] + buf[(i - 3) % 7];
        v -= 2 * buf[(i - 4) % 7];
        v -= buf[(i - 5) % 7];
        v -= buf[(i - 6) % 7];
        buf[i % 7] = v;
    }
    return buf[m % 7];
}
