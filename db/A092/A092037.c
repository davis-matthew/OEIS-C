/*
 * A092037  A092255 mod 3.
 * https://oeis.org/A092037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a power of 3 (or n == 0); a(n) = 2 if n
 *            consists only of 0 and 1 in base‑3 and is not a power of 3; a(n)
 *            = 0 otherwise. This matches the OEIS definition of A092037/
 *            A092255 mod 3
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   has_only_01_base3, is_power_of_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_only_01_base3.h"
#include "../utils/is_power_of_3.h"

/* Main sequence function.  External linkage as required. */
seq_t a(idx_t n) {
    if (n == 0)          /* Special case present in the OEIS data */
        return (seq_t)1;

    if (is_power_of_3(n))
        return (seq_t)1;

    if (has_only_01_base3(n))
        return (seq_t)2;

    return (seq_t)0;
}
