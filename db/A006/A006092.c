/*
 * A006092  Numbers beginning with letter 't' when spelled out in English.
 * https://oeis.org/A006092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   search: a(n) is the n-th k >= 1 whose leading 3-digit group (k /
 *            1000^j, below 1000) is spelled with a leading 't': 2, 3, 10, 12,
 *            13, 20-39, 200-399
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   group_starts_with_t, starts_with_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/group_starts_with_t.h"
#include "../utils/starts_with_t.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t i = 0;
    while (count < n) {
        i++;
        if (starts_with_t(i)) count++;
    }
    return i;
}
