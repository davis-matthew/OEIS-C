/*
 * A227781  Least number of squares which add to -1 mod n.
 * https://oeis.org/A227781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n>1: if n%8==0 -> 4; else if n%4==0 -> 3; else if
 *            all odd prime factors of n ≡1 (mod4) -> 1; else -> 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   odd_factors_all_one_mod_four
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/odd_factors_all_one_mod_four.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;

    /* Handle even values first */
    if ((n & 1) == 0) {
        if ((n & 3) == 0) { /* divisible by 4 */
            if ((n & 7) == 0) return (seq_t)4;   /* n % 8 == 0 */
            return (seq_t)3;                     /* n % 4 == 0 but n % 8 != 0 */
        }
        /* n is 2 mod 4 */
        /* Remove all factors of 2 to get the odd part */
        idx_t odd = n;
        while ((odd & 1) == 0) odd >>= 1;
        if (odd_factors_all_one_mod_four(odd)) return (seq_t)1;
        return (seq_t)2;
    }

    /* n is odd */
    if (odd_factors_all_one_mod_four(n)) return (seq_t)1;
    return (seq_t)2;
}
