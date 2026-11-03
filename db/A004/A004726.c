/*
 * A004726  Delete all 7's from the sequence of nonnegative integers.
 * https://oeis.org/A004726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   search: enumerate k = 0, 1, 2, ..., delete every digit 7 from k,
 *            skip k whose digits are all 7 (nothing left); a(n) is the n-th
 *            surviving value (leading zeros dropped, e.g. 17 -> 1, 70 -> 0)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool all_sevens(uint64_t k) {
    if (k == 0) return false;
    while (k > 0) {
        if (k % 10 != 7) return false;
        k /= 10;
    }
    return true;
}

static seq_t strip_sevens(uint64_t k) {
    seq_t result = 0;
    seq_t power = 1;
    while (k > 0) {
        unsigned d = (unsigned)(k % 10);
        if (d != 7) {
            result += power * d;
            power *= 10;
        }
        k /= 10;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t k = 0;
    idx_t count = 0;
    for (;; k++) {
        if (all_sevens(k)) continue;
        if (count == n) return strip_sevens(k);
        count++;
    }
}
