/*
 * A043625  Numbers whose base-8 representation has exactly 3 runs.
 * https://oeis.org/A043625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth integer whose base‑8 representation has exactly
 * three runs (maximal blocks of equal consecutive digits).  The runs may
 * be of any positive length and the digits may be 0–7 with the first
 * digit non‑zero.
 *
 * The implementation simply scans integers in increasing order, counting
 * how many base‑8 representations contain exactly two transitions between
 * adjacent digits (hence three runs).  When the desired count is reached,
 * the corresponding integer is returned.
 */

#include <stdbool.h>

/* Return true iff val has exactly three runs in base‑8. */
static inline bool has_three_runs(seq_t val)
{
    if (val < 8)            /* single digit -> only one run */
        return false;

    seq_t tmp = val;
    uint8_t prev = (uint8_t)(tmp % 8);
    tmp /= 8;

    int transitions = 0;    /* number of digit changes seen */
    while (tmp > 0) {
        uint8_t cur = (uint8_t)(tmp % 8);
        if (cur != prev) {
            ++transitions;
            if (transitions > 2)   /* early exit */
                return false;
            prev = cur;
        }
        tmp /= 8;
    }
    return transitions == 2;    /* exactly two changes -> three runs */
}

/*
 * Compute the nth term of A043625 (1‑based indexing).
 * The function works for any 64‑bit or arbitrary‑precision seq_t.
 */
seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;

    while (1) {
        if (has_three_runs(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
