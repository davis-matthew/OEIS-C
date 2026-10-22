/*
 * A007908  Triangle of the gods: to get a(n), concatenate the decimal numbers
 *          1,2,3,...,n.
 * https://oeis.org/A007908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, changed
 * Formula:   a(n) = n + a(n-1)*10^A055642(n), a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
static int count_digits(idx_t n) {
    if (n == 0) return 1;
    int count = 0;
    while (n > 0) {
        count++;
        n /= 10;
    }
    return count;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    for (idx_t i = 2; i <= n; i++) {
        int digits = count_digits(i);
        result = (seq_t)i + result * pow10(digits);
    }
    return result;
}
