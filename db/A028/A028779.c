/*
 * A028779  Nonsquares mod 66.
 * https://oeis.org/A028779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonsquares mod 66: a(n) is the n-th positive integer that is not a quadratic residue modulo 66. */
#include <stdbool.h>
#include <stdint.h>

static uint8_t _squares_66[66];
static bool _init_66 = false;

static void _init_squares_66(void) {
    for (int i = 0; i < 66; ++i) {
        _squares_66[i] = 0;
    }
    for (int i = 0; i < 66; ++i) {
        int r = (i * i) % 66;
        _squares_66[r] = 1;
    }
    _init_66 = true;
}

seq_t a(idx_t n) {
    if (!_init_66)
        _init_squares_66();

    idx_t count = 0;
    idx_t cand = 1;
    for (;;) {
        idx_t rem = cand % 66;
        if (!_squares_66[rem]) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
