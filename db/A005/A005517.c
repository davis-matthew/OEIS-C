/*
 * A005517  Smallest label f(T) given to a rooted tree T with n nodes in
 *          Matula-Goebel labeling.
 * https://oeis.org/A005517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1)=1; a(2)=2; a(n) = 3*5^((n-3)/3) if n=0 (mod 3);
 *            a(n)=5^((n-1)/3) if n>=4 and n=1 (mod 3); a(n)=9*5^((n-5)/3) if
 *            n>=5 and n = 2 (mod 3)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    
    seq_t five = (seq_t)5;
    idx_t rem = n % 3;
    
    if (rem == 0) {
        idx_t exp = (n - 3) / 3;
        return (seq_t)3 * ipow_idx(five, exp);
    } else if (rem == 1) {
        idx_t exp = (n - 1) / 3;
        return ipow_idx(five, exp);
    } else {
        idx_t exp = (n - 5) / 3;
        return (seq_t)9 * ipow_idx(five, exp);
    }
}
