/*
 * A158572  a(n) = A007814((2n-1)!! - 1).
 * https://oeis.org/A158572
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007814((2n-1)!! - 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 2) return 0;            /* offset is 2; below 2 is irrelevant */

    uint64_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        uint64_t odd = (uint64_t)2 * i - 1;          /* odd factor */
        prod = (uint64_t)((__uint128_t)prod * odd); /* product mod 2^64   */
    }

    uint64_t val = prod - 1;         /* compute (2n-1)!! - 1  */
    unsigned cnt = 0;
    while ((val & 1) == 0 && val != 0) {   /* count trailing zeros   */
        ++cnt;
        val >>= 1;
    }

    return (seq_t)cnt;               /* v₂((2n-1)!! - 1)      */
}
