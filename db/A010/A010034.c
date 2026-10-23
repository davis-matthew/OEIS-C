/*
 * A010034  Numbers k such that gcd(k^17 + 9, (k+1)^17 + 9) > 1.
 * https://oeis.org/A010034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8424432925592889329288197322308900672459420460792433 +
 *            8936582237915716659950962253358945635793453256935559*(n-1)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

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
