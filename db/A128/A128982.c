/*
 * A128982  If in a line of n persons every n-th person is eliminated until
 *          only one person is left, which position P should one assume in the
 *          original lineup to avoid being eliminated?
 * https://oeis.org/A128982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = survivor when repeatedly removing every n‑th person *
 *            from a list of n persons, restarting counting at the lowest *
 *            remaining number each round. It can be computed in O(n) * time
 *            by the recurrence * s_1 = 0 * for m = 2 .. n: * e = (n-1) mod m
 *            // index of removed person (0‑based) * if s_{m-...
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 1;                 /* a(1) = 1 */
    idx_t survivor = 0;                    /* index in the current list (0‑based) */
    for (idx_t m = 2; m <= n; ++m) {
        idx_t e = (n - 1) % m;             /* index of the person eliminated now */
        if (survivor >= e) survivor += 1;  /* map survivor index back to original list */
    }
    return (seq_t)(survivor + 1);          /* convert to 1‑based position */
}
