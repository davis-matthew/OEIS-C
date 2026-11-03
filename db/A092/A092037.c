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
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether n has only digits 0 or 1 in base‑3. */
static bool has_only_01_base3(idx_t n) {
    if (n == 0) return true;          /* 0 is represented as a single 0 */
    while (n > 0) {
        if (n % 3 == 2)
            return false;
        n /= 3;
    }
    return true;
}

/* Test whether n is a power of 3 (n > 0). */
static bool is_power_of_3(idx_t n) {
    if (n == 0) return false;
    while (n % 3 == 0)
        n /= 3;
    return n == 1;
}

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
