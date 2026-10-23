/*
 * A032733  Numbers k such that k prefixed by '8' and followed by '9' is
 *          prime.
 * https://oeis.org/A032733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer k such that the decimal number formed
 *            by * prefixing k with digit 8 and suffixing it with digit 9 is
 *            prime
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 28, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1, but keep defined for safety */
    idx_t k = 0;
    idx_t count = 0;
    while (1) {
        /* Number of decimal digits in k (k=0 has 1 digit) */
        int d = digits(k);
        /* Construct number N = 8 * 10^(d+1) + k * 10 + 9 */
        seq_t N = (seq_t)8 * pow10((idx_t)d + 1) + ((seq_t)k * 10) + 9;
        if (is_prime_big(N)) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
        k++;
    }
}
