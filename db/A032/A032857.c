/*
 * A032857  Numbers whose base-10 representation Sum_{i=0..m} d(i)*10^i, d(m)
 *          > 0, has d(m) <= d(m-1) >= d(m-2) <= ...
 * https://oeis.org/A032857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose decimal digits form an alternating *
 *            pattern starting with ‘≤’ from the most significant digit: * for
 *            digits d(m)…d(0) the inequalities d(m) ≤ d(m‑1) ≥ d(m‑2) ≤ …
 *            hold
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Test whether a number satisfies the alternating digit condition. */
static bool valid(seq_t v) {
    int digits[64];
    int len = 0;
    seq_t t = v;
    while (t > 0) {
        digits[len++] = (int)(t % 10);
        t /= 10;
    }
    /* Check inequalities from most significant to least significant. */
    bool le = true; /* first comparison is ≤ */
    for (int i = len - 1; i > 0; --i) {
        if (le) {
            if (digits[i] > digits[i - 1]) return false;
        } else {
            if (digits[i] < digits[i - 1]) return false;
        }
        le = !le;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t x = 1;
    while (1) {
        if (valid(x)) {
            count++;
            if (count == n) return x;
        }
        x += 1;
    }
}
