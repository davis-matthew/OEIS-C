/*
 * A006715  Describe the previous term! (method A - initial term is 3).
 * https://oeis.org/A006715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   look-and-say sequence with a(1) = 3; a(n+1) obtained by reading
 *            digits of a(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   build_from_digits, extract_digits, look_and_say
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_from_digits.h"
#include "../utils/extract_digits.h"
#include "../utils/look_and_say.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return (seq_t)3;
    }
    
    seq_t current = (seq_t)3;
    for (idx_t i = 2; i <= n; i++) {
        current = look_and_say(current);
    }
    
    return current;
}
