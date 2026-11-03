/*
 * A010034  Numbers k such that gcd(k^17 + 9, (k+1)^17 + 9) > 1.
 * https://oeis.org/A010034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8424432925592889329288197322308900672459420460792433 +
 *            8936582237915716659950962253358945635793453256935559*(n-1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Validated: standard, pass, 20 terms through n = 90 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

seq_t a(idx_t n)
{
    static seq_t first = 0;
    static seq_t diff  = 0;
    static bool init = false;
    if (!init) {
        first = seq_from_str("8424432925592889329288197322308900672459420460792433");
        diff  = seq_from_str("8936582237915716659950962253358945635793453256935559");
        init = true;
    }
    seq_t nm1 = (seq_t)n - 1;
    return first + nm1 * diff;
}
