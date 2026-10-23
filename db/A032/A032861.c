/*
 * A032861  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(m)
 *          > d(m-1) < d(m-2) > ...
 * https://oeis.org/A032861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032861: numbers whose base‑6 digits alternate
   as d_m > d_{m-1} < d_{m-2} > … (msd first).  The function
   generates terms by brute‑force checking. */
#include <stddef.h>
#include <stdbool.h>

/* Return true iff x satisfies the alternating inequality condition. */
static inline bool is_alternating(seq_t x)
{
    /* 0 has a single digit, hence is valid. */
    if (x == (seq_t)0)
        return true;

    /* Extract base‑6 digits (least significant first). */
    uint8_t digits[64];
    size_t len = 0;
    seq_t tmp = x;
    while (tmp != (seq_t)0) {
        digits[len++] = (uint8_t)(tmp % (seq_t)6);
        tmp /= (seq_t)6;
    }

    /* Compare digits from most significant to least significant. */
    bool greater = true;  /* next comparison: prev > cur */
    seq_t prev = (seq_t)digits[len - 1];
    for (int i = (int)len - 2; i >= 0; --i) {
        seq_t cur = (seq_t)digits[i];
        if (greater) {
            if (!(prev > cur))
                return false;
        } else {
            if (!(prev < cur))
                return false;
        }
        greater = !greater;
        prev = cur;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t cur = (seq_t)0;

    while (count < n) {
        if (is_alternating(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
    /* Unreachable, but satisfies the compiler. */
    return (seq_t)0;
}
