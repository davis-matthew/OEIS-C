/*
 * A056525  Palindromes with odd number of digits.
 * https://oeis.org/A056525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(n/10)*10^digitcount(n) + rev(n) * where
 *            digitcount(n) is the number of decimal digits of n * and rev(n)
 *            is the decimal reverse of n (leading zeros are dropped)
 * Range:     u8 n <= 25, u16 n <= 654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_idx.h"
static unsigned digit_count_idx(idx_t n) {
    unsigned count = 0;
    do {
        ++count;
        n /= 10;
    } while (n);
    return count;
}
 

 
seq_t a(idx_t n) {
    seq_t prefix = (seq_t)(n / 10);
    unsigned d = digit_count_idx(n);
    seq_t pow10 = 1;
    for (unsigned i = 0; i < d; ++i) {
        pow10 = pow10 * 10;
    }
    seq_t rev = reverse_idx(n);
    return prefix * pow10 + rev;
}
