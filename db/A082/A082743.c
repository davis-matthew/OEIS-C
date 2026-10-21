/*
 * A082743  a(0)=1, a(1)=2; for n >= 2, a(n) is smallest palindrome greater
 *          than 1 which is congruent to 1 (mod n).
 * https://oeis.org/A082743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A082743:
   a(0)=1, a(1)=2; for n ≥ 2, a(n) is the smallest palindrome >1
   congruent to 1 (mod n).  Implementation uses a simple search:
   increment candidates until both the modulo and palindrome tests
   are satisfied. */
#include <stdbool.h>
#include "../utils/is_palindrome.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t candidate = (seq_t)2;
    const seq_t one = (seq_t)1;
    for ( ;; candidate++ ) {
        if ( (candidate % (seq_t)n) == one ) {
            if ( is_palindrome(candidate, (idx_t)10) )
                return candidate;
        }
    }
}
