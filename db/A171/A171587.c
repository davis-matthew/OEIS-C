/*
 * A171587  Sequence of the diagonal variant of the Fibonacci word fractal.
 *          Sequence of the Fibonacci tile.
 * https://oeis.org/A171587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*phi^2) mod 2
 * Range:     u8 n <= 3184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t getbit(idx_t m, seq_t pos, seq_t *lens) {
    if (m == 1) return (seq_t)0;
    if (pos < lens[m-1]) {
        return getbit(m-1, pos, lens);
    } else {
        pos -= lens[m-1];
        if ((m % 3) == 2) {
            return getbit(m-2, pos, lens);
        } else { /* m%3 == 0 or 1, apply bar (flip) */
            seq_t r = getbit(m-2, pos, lens);
            return r ^ (seq_t)1;
        }
    }
}

seq_t a(idx_t n) {
    seq_t pos = (seq_t)n;
    /* Small static buffer is enough for all reasonable n.
       For 64‑bit indices the required index m is below 100. */
    const idx_t MAX_M = 200;
    seq_t lens[MAX_M];
    lens[0] = (seq_t)0;
    lens[1] = (seq_t)1;
    idx_t i = 1;
    while (lens[i] <= pos) {
        if ((i + 1) >= MAX_M) break;
        lens[i + 1] = lens[i] + lens[i - 1];
        i++;
    }
    idx_t m = i;  /* lens[m] > pos  (unless MAX_M was reached) */
    return getbit(m, pos, lens);
}
