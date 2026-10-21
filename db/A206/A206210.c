/*
 * A206210  Number of (n+1) X 4 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two equal edges, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A206210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18 * a(n-3) for n > 6 a(1) = 162, a(2) = 336, a(3) = 760,
 *            a(4) = 2224, a(5) = 5632, a(6) = 14416
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static const seq_t base[7] = {
    0,         /* unused index 0 */
    162,       /* a(1) */
    336,       /* a(2) */
    760,       /* a(3) */
    2224,      /* a(4) */
    5632,      /* a(5) */
    14416      /* a(6) */
};

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* undefined for n < 1 */

    if (n <= 6)
        return base[(size_t)n];/* return the pre‑computed values */

    /* n >= 7 : use recurrence a(n) = 18 * a(n-3) repeatedly */
    /* number of 18-multiplications needed  */
    idx_t exp = (n - 7) / 3 + 1;

    /* compute 18^exp */
    seq_t pow = 1;
    for (idx_t i = 0; i < exp; ++i)
        pow *= (seq_t)18;

    /* base index: 4 + ((n-1) % 3) yields 4,5,6 cyclically */
    idx_t bi = 4 + ((n - 1) % 3);

    return base[(size_t)bi] * pow;
}
