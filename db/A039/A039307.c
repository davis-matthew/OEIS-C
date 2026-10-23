/*
 * A039307  Numbers whose base-6 representation has the same nonzero number of
 *          4's and 5's.
 * https://oeis.org/A039307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th positive integer whose base‑6 representation has *
 *            the same nonzero number of 4’s and 5’s. (a(1) = 29, a(2) = 34,
 *            …) * * The implementation simply scans successive integers,
 *            checks the * condition, and returns the n‑th one found. For n up
 *            to about 10 000 * this is well below the 2 s t...
 * Range:     u8 n <= 26, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* test whether the base‑6 digits of x contain the same nonzero number
 * of 4's and 5's */
static inline bool satisfies(seq_t x)
{
    seq_t   t   = x;
    uint8_t cnt4 = 0, cnt5 = 0;
    const seq_t six = (seq_t)6;

    while (t > 0) {
        uint8_t d = (uint8_t)(t % six);
        if (d == 4)
            ++cnt4;
        else if (d == 5)
            ++cnt5;
        t /= six;
    }
    return cnt4 > 0 && cnt4 == cnt5;
}

/* public sequence function */
seq_t a(idx_t n)
{
    /* the OEIS offset is 1, but the function is only called with n >= 1.
     * For n==0 we simply return 0 – the value is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    idx_t found = 0;
    idx_t cand  = 1;

    for (;;) {
        seq_t val = (seq_t)cand;
        if (satisfies(val)) {
            ++found;
            if (found == n)
                return val;
        }
        ++cand;
    }
}
