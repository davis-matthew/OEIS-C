/*
 * A005819  Number of words of length n in a certain language.
 * https://oeis.org/A005819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -1 + Sum_{i=1..n} (n-i+1)*phi(i) for n >= 2, a(1) = 1
 * Range:     u8 n <= 12, u16 n <= 85, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   a005819_totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005819_totient.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return 1;
    }
    seq_t result = 0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t coeff = (seq_t)(n - i + 1);
        seq_t ph = (seq_t)a005819_totient((uint64_t)i);
        result += coeff * ph;
    }
    result -= 1;
    return result;
}
