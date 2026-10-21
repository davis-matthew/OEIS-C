/*
 * A086159  Number of partitions of n into the first three triangular numbers,
 *          1, 3 and 6.
 * https://oeis.org/A086159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (floor(n/6)+1) * (floor(n/6)+floor((n%6)/3)+1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the number of partitions of n into the parts 1, 3, and 6. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute q = floor(n / 6) and r = n mod 6 */
    idx_t q = n / 6U;
    idx_t r = n % 6U;
    idx_t s = r / 3U;  /* floor(r / 3) : 0 for r = 0..2, 1 for r = 3..5 */

    /* Result: (q + 1) * (q + s + 1). Cast operands to seq_t to handle
       arbitrary‑precision builds without overflow on intermediate idx_t. */
    return (seq_t)(q + 1U) * (seq_t)(q + s + 1U);
}
