/*
 * A065807  Squares with a smaller square as final digits.
 * https://oeis.org/A065807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 219, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A065807: Squares with a smaller square as final digits
 *  a(n) = the n-th perfect square m² whose decimal representation
 *         has a proper suffix that is itself a perfect square.
 *  The suffix may be any length from 1 to (digits−1) and may be 0.
 *  The function searches successive squares until it finds the n‑th one.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root (floor) of a seq_t value */
static seq_t seq_isqrt(seq_t v) {
    if (v < (seq_t)2) return v;        /* 0 or 1 */
    seq_t low = 1;
    seq_t high = v / 2;                /* sqrt(v) <= v/2 for v>=4 */
    seq_t best = 1;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t div = v / mid;
        if (mid <= div) {              /* mid*mid <= v */
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* true if sq has a proper suffix that is a perfect square (including 0) */
static bool has_square_suffix(seq_t sq) {
    if (sq < (seq_t)10) return false;   /* need at least two digits */

    /* count decimal digits of sq */
    seq_t tmp = sq;
    int digits = 0;
    while (tmp) { tmp /= 10; digits++; }
    if (digits < 2) return false;       /* safety, though handled above */

    /* iterate over possible suffix lengths */
    seq_t power = 10;                   /* 10^1 initially */
    for (int len = 1; len < digits; ++len) {
        seq_t suffix = sq % power;
        if (suffix == 0) return true;   /* 0 = 0², always qualifies */

        seq_t r = seq_isqrt(suffix);
        if (r * r == suffix) return true;

        power *= 10;                    /* next power of 10 */
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* Undefined for n=0, but harmless */
    seq_t count = 0;
    seq_t m = 2;                   /* start from 2 since 1² has no proper suffix */

    while (true) {
        seq_t sq = m * m;
        if (has_square_suffix(sq)) {
            count++;
            if (count == n) return sq;
        }
        m++;
    }
}
