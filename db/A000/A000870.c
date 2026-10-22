/*
 * A000870  Numbers beginning with letter 's' in English.
 * https://oeis.org/A000870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   numbers beginning with letter 's' in English when spelled out
 * Range:     u8 n <= 24, u16 n <= 9760, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   starts_with_s, starts_with_s_group
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/starts_with_s.h"
#include "../utils/starts_with_s_group.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t num = 0;
    while (count < n) {
        num++;
        if (starts_with_s(num)) {
            count++;
        }
    }
    return num;
}
