/*
 * A117888  Period of sequence {Kronecker(n,k), k = 1, 2, 3, ...}, or 0 if the
 *          sequence is not periodic.
 * https://oeis.org/A117888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = period of sequence Kronecker(n,k), k=1,2,3,... a(n) = 4 *
 *            rad(n) if n = 2^t * s, t odd a(n) = rad(n) if t even and s ≡ 1
 *            (mod 4) a(n) = 2 * rad(n) if t even, t>0, s ≡ 3 (mod 4) a(n) = 0
 *            if t = 0 and s ≡ 3 (mod 4) where rad(n) = product of distinct
 *            prime divisors of n
 * Range:     u8 n <= 65, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   _div2, _rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_div2.h"
#include "../utils/_rad.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for offset 0, keep safe */

    uint64_t t, odd_part;
    _div2((uint64_t)n, &t, &odd_part);

    seq_t rad = _rad((uint64_t)n);

    if (t & 1ULL) {                 /* t odd */
        return rad * (seq_t)4;
    } else {                         /* t even */
        if (t == 0) {                /* n odd */
            if ((odd_part & 3ULL) == 3ULL) return 0;
            return rad;
        } else {                     /* t ≥ 2 */
            if ((odd_part & 3ULL) == 3ULL) return rad * (seq_t)2;
            return rad;
        }
    }
}
