/*
 * A064704  Numbers beginning and ending with their multiplicative digital
 *          root.
 * https://oeis.org/A064704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   A064704 – numbers beginning and ending with their multiplicative
 *            digital root
 * Range:     u8 n <= 13, u16 n <= 133, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_valid, mul_digital_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_valid.h"
#include "../utils/mul_digital_root.h"

/* Main sequence function.  Generates terms on the fly by brute force search,
   which is fast enough for n up to about 10 000. */
seq_t a(idx_t n) {
    idx_t count = 0;
    for (uint64_t cand = 1; ; ++cand) {
        if (is_valid(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
    }
}
