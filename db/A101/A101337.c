/*
 * A101337  Sum of (each digit of n raised to the power (number of digits in
 *          n)).
 * https://oeis.org/A101337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A101337: sum of each decimal digit of n raised to the power of
 * the number of digits in n
 *
 * Formula: a(n) = Σ_{d∈digits(n)} d^{digits(n)}   (OEIS notation)
 *
 * This implementation works for any seq_t type that supports the
 * required integer operations.
 */
#include <stdint.h>
#include <stdbool.h>

/* External entry point as required */
seq_t a(idx_t n) {
    /* Handle offset: a(1) is defined, a(0) is not used but return 0 */
    if (n == 0)
        return 0;

    /* Count digits of n */
    int digits = 0;
    idx_t tmp = n;
    do {
        digits++;
        tmp /= 10;
    } while (tmp);

    /* Compute sum of each digit raised to the power of number of digits */
    seq_t sum = 0;
    tmp = n;
    do {
        int d = tmp % 10;
        /* Compute d^digits using repeated multiplication in seq_t */
        seq_t pow_val = 1;
        for (int i = 0; i < digits; ++i)
            pow_val *= (seq_t)d;
        sum += pow_val;
        tmp /= 10;
    } while (tmp);

    return sum;
}
