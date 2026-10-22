/*
 * A000993  Number of distinct quadratic residues mod 10^n; also number of
 *          distinct n-digit endings of base-10 squares.
 * https://oeis.org/A000993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, base
 * Formula:   a(n) = s2(n)*s5(n), where s_p(n) = number of squares mod p^n = 1
 *            + Sum_{m = n, n-2, ..., m >= 1} U_p(m), with U_p(m) the number
 *            of unit squares mod p^m: U_5(m) = 2*5^(m-1); U_2(1) = U_2(2) =
 *            1, U_2(m) = 2^(m-3) for m >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   a000993_count2, a000993_count5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000993_count2.h"
#include "../utils/a000993_count5.h"

seq_t a(idx_t n) {
    return a000993_count2(n) * a000993_count5(n);
}
