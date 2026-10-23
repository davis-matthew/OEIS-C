/*
 * A044732  Numbers n such that string 1,9 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that the decimal representation of n contains the
   substring "19" while the decimal representation of n+1 does not.
   The function returns the nth such number (OEIS A044732). */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

/* Helper: true if decimal form of v contains the consecutive digits 1 and 9. */
static bool has_substring_19(uint64_t v) {
    char buf[32];
    int len = snprintf(buf, sizeof buf, "%llu", (unsigned long long)v);
    for (int i = 0; i + 1 < len; ++i) {
        if (buf[i] == '1' && buf[i + 1] == '9')
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;                       /* Undefined for the offset 0, return 0 safely */

    idx_t found = 0;
    uint64_t cur = 0;
    while (found < n) {
        ++cur;
        if (has_substring_19(cur) && !has_substring_19(cur + 1))
            ++found;
    }
    return (seq_t)cur;
}
