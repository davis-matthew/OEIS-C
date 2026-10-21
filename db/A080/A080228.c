/*
 * A080228  Numbers containing the digits 0, 1, 2, 5 or 8 only.
 * https://oeis.org/A080228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n‑th non‑negative integer (a(1)=0) whose decimal
 *            digits belong to {0,1,2,5,8}
 * Range:     u8 n <= 69, u16 n <= 2500, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */

/* Local helper: test whether all decimal digits of x are 0,1,2,5 or 8. */
static inline bool allowed_digits(seq_t x)
{
    if (x == 0) return true;              /* zero itself is allowed */
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        if (d != 0 && d != 1 && d != 2 && d != 5 && d != 8) return false;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t val   = 0;   /* candidate value */
    idx_t cnt   = 0;   /* how many admissible numbers found so far */

    while (1) {
        if (allowed_digits(val)) {
            ++cnt;
            if (cnt == n) return val;      /* found the n‑th term */
        }
        ++val;                               /* try next integer */
    }
}
