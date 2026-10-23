/*
 * A152463  Numbers k such that 4 + 5*k^3 is a square.
 * https://oeis.org/A152463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 4, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for k such that 4 + 5*k^3 is a perfect square */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Store the solutions found on first call */
    static seq_t solbuf[4];
    static size_t solcnt = 0;
    static bool inited = false;

    if (!inited) {
        /* Limit chosen to cover the largest known solution (532)
           and to avoid any 64‑bit overflow (5*k^3 <= 4·10^16). */
        const idx_t LIMIT = 200000ULL;
        for (idx_t k = 0; k <= LIMIT; ++k) {
            unsigned long long val = 5ULL * k * k * k + 4ULL;
            seq_t s = floor_sqrt((seq_t)val);
            if ((seq_t)s * s == (seq_t)val) {
                if (solcnt < sizeof(solbuf)/sizeof(solbuf[0])) {
                    solbuf[solcnt++] = (seq_t)k;
                }
            }
        }
        inited = true;
    }

    if (n >= 1 && n <= solcnt) {
        return solbuf[n - 1];
    }
    return 0;
}
