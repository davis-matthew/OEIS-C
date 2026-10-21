/*
 * A079807  Number of multiples of 3 that can be formed by permuting the
 *          digits of n.
 * https://oeis.org/A079807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 0 if n mod 3 ≠ 0; otherwise a(n) = multinomial(m;
 *            c(0),...,c(9)) where m = Σ c(i) is the number of digits of n and
 *            c(i) is the number of occurrences of digit i. This counts all
 *            distinct permutations of the digits of n, which are all
 *            divisible by 3 because n itself is divisible by 3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* If n is not a multiple of 3, no permutation can change the digit sum,
       hence the result is zero. */
    if (n % 3 != 0) {
        return 0;
    }

    /* Count decimal digits of n. Special case n==0. */
    int count[10] = {0};
    if (n == 0) {
        count[0] = 1;
    } else {
        uint64_t tmp = n;
        while (tmp > 0) {
            count[tmp % 10] += 1;
            tmp /= 10;
        }
    }

    /* Total number of digits. */
    int m = 0;
    for (int d = 0; d < 10; ++d) {
        m += count[d];
    }

    /* Compute the multinomial coefficient as a product of binomial coefficients,
       avoiding intermediate factorial growth. */
    seq_t result = 1;
    int remaining = m;
    for (int d = 0; d < 10; ++d) {
        int c = count[d];
        if (c == 0) {
            continue;
        }
        /* binomial(remaining, c) */
        seq_t bin = 1;
        for (int k = 1; k <= c; ++k) {
            bin = bin * (seq_t)(remaining - k + 1) / (seq_t)k;
        }
        result *= bin;
        remaining -= c;
    }

    return result;
}
