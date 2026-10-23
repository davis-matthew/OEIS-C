/*
 * A036691  Compositorial numbers: product of first n composite numbers.
 * https://oeis.org/A036691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = product of the first n composite numbers, with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t prod = (seq_t)1;
    idx_t count = 0;
    idx_t i = 4; /* 4 is the first composite number */

    while (count < n) {
        if (!is_prime(i)) {
            prod = seq_mul(prod, (seq_t)i);
            count++;
        }
        i++;
    }

    return prod;
}
