/*
 * A237545  Odious powers of 3.
 * https://oeis.org/A237545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th power of 3 that is odious (has an odd number of
 *            1‑bits in its binary representation)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool parity_of(seq_t x)
{
    bool p = false;
    while (x != (seq_t)0) {
        if ((x & (seq_t)1) != (seq_t)0)
            p = !p;
        x >>= 1;
    }
    return p;
}

seq_t a(idx_t n)
{
    /* generate powers of 3 until we have n odious terms */
    seq_t val = (seq_t)1;      /* 3^0 */
    idx_t  count = 1;          /* first term already found */

    if (n == 1)
        return val;

    for (idx_t k = 1; count < n; ++k) {
        val *= (seq_t)3;      /* next power of 3 */
        if (parity_of(val)) {
            ++count;
            if (count == n)
                return val;
        }
    }

    /* control should never reach here for valid n >= 1 */
    return val;
}
