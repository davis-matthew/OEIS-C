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
 * Helpers:   all_sevens, strip_sevens
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/all_sevens.h"
#include "../utils/strip_sevens.h"

seq_t a(idx_t n) {
    uint64_t k = 0;
    idx_t count = 0;
    for (;; k++) {
        if (all_sevens(k)) continue;
        if (count == n) return strip_sevens(k);
        count++;
    }
}
