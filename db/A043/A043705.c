/*
 * A043705  Numbers whose base-16 representation has an even number of runs.
 * https://oeis.org/A043705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9524 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043705: Numbers whose base‑16 representation has an even number of runs
   (enumerated in increasing order). This implementation scans integers
   and counts the runs in their hexadecimal representation.  */
#include <stdbool.h>

static inline bool even_hex_runs(idx_t num)
{
    /* If num is 0, we do not consider it (offset starts at 16). */
    if (num == 0) return false;

    uint8_t prev = (uint8_t)(num & 0xFu);   /* first hex digit */
    num >>= 4;
    uint8_t runs = 1;                       /* at least one run */

    while (num != 0) {
        uint8_t d = (uint8_t)(num & 0xFu);
        if (d != prev) {
            runs++;
            prev = d;
        }
        num >>= 4;
    }
    return (runs & 1u) == 0;                /* even number of runs */
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;        /* start inspecting from 1 */

    while (1) {
        if (even_hex_runs(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;   /* cast to return type */
        }
        candidate++;
    }
}
