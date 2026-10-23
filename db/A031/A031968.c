/*
 * A031968  Numbers with exactly four distinct base-9 digits.
 * https://oeis.org/A031968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2688, u32 n <= 2688, u64 n <= 2688,
 *            u128 n <= 2688, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating all 4‑digit base‑9 numbers with distinct digits. */
#include <stddef.h>
#include <stdbool.h>

#define TOTAL 2688

seq_t a(idx_t n)
{
    static seq_t seq[TOTAL];
    static bool initialized = false;

    if (!initialized) {
        size_t pos = 0;
        for (idx_t d0 = 1; d0 < 9; ++d0) {
            for (idx_t d1 = 0; d1 < 9; ++d1) {
                if (d1 == d0) continue;
                for (idx_t d2 = 0; d2 < 9; ++d2) {
                    if (d2 == d0 || d2 == d1) continue;
                    for (idx_t d3 = 0; d3 < 9; ++d3) {
                        if (d3 == d0 || d3 == d1 || d3 == d2) continue;
                        seq[pos++] = (seq_t)d0 * 729 + (seq_t)d1 * 81
                                   + (seq_t)d2 * 9 + (seq_t)d3;
                    }
                }
            }
        }
        initialized = true;
    }

    if (n == 0 || n > TOTAL) return 0;
    return seq[(size_t)n - 1];
}
