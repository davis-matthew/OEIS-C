/*
 * A006720  Somos-4 sequence: a(0)=a(1)=a(2)=a(3)=1; for n >= 4, a(n) =
 *          (a(n-1) * a(n-3) + a(n-2)^2) / a(n-4).
 * https://oeis.org/A006720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (a(n-1)*a(n-3) + a(n-2)^2) / a(n-4), a(0) = a(1) = a(2) =
 *            a(3) = 1
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 16, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

seq_t a(idx_t n) {
    if (n < 4) return (seq_t)1;

    if (seq_is_big()) {
        static seq_t b0, b1, b2, b3;   /* a(k-3), a(k-2), a(k-1), a(k) */
        static idx_t bk = 0;
        if (bk == 0 || n < bk) {
            b0 = 1; b1 = 1; b2 = 1; b3 = 1;
            bk = 3;
        }
        while (bk < n) {
            seq_t t = (b3 * b1 + b2 * b2) / b0;
            b0 = b1; b1 = b2; b2 = b3; b3 = t;
            bk++;
        }
        return b3;
    } else {
        /* fixed width: keep intermediates in 128 bits so the products
           do not overflow for every term that fits in 64 bits */
        static unsigned __int128 u0, u1, u2, u3;
        static idx_t uk = 0;
        if (uk == 0 || n < uk) {
            u0 = 1; u1 = 1; u2 = 1; u3 = 1;
            uk = 3;
        }
        while (uk < n) {
            unsigned __int128 num = u3 * u1 + u2 * u2;
            unsigned __int128 t = (u0 != 0) ? num / u0 : 0;
            u0 = u1; u1 = u2; u2 = u3; u3 = t;
            uk++;
        }
        return u128_to_seq(u3);
    }
}
