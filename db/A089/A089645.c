/*
 * A089645  Penny Flipping, or Flipping Coins: Given a stack of n coins, flip
 *          the top coin, then the stack of the top two coins, then the stack
 *          of the top three etc... starting again with the top coin after
 *          flipping all n coins. A flip of m coins reverses their order and
 *          inverts their state. This is the numb...
 * https://oeis.org/A089645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = n*m if 2^m == 1 (mod 2n+1), else n*m - 1, where m =
 *            A003558(n) is the least m>0 with 2^m == +-1 (mod 2n+1); a(1) = 2
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return 2;
    seq_t p = (seq_t)n * 2 + 1;
    seq_t power = 2 % p;
    seq_t m = 1;
    while (power != 1 && power != p - 1) {
        power = (power * 2) % p;
        m++;
    }
    seq_t r = (seq_t)n * m;
    if (power != 1) r -= 1;
    return r;
}
