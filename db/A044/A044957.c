/*
 * A044957  Numbers with no two equally numerous base 8 digits.
 * https://oeis.org/A044957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer k such that in base‑8 * each
 *            digit that occurs appears a distinct number of times (no two *
 *            non‑zero digit counts are equal)
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool valid_base8(seq_t x) {
    unsigned char count[8] = {0};
    unsigned char seen[64] = {0};
    seq_t tmp = x;
    while (tmp > 0) {
        unsigned int d = (unsigned int)(tmp % ((seq_t)8));
        count[d]++;
        tmp /= ((seq_t)8);
    }
    for (int i = 0; i < 8; ++i) {
        if (count[i] > 0) {
            if (seen[(size_t)count[i]]) return false;
            seen[(size_t)count[i]] = 1;
        }
    }
    return true;
}

seq_t a(idx_t n) {
    seq_t candidate = 1;
    idx_t found = 0;
    while (found < n) {
        if (valid_base8(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
    return 0; /* Should never be reached */
}
