/*
 * A032933  Numbers whose set of base-13 digits is {1,2}.
 * https://oeis.org/A032933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑13 representation
 *            consists solely of the digits 1 and 2. The sequence is sorted in
 *            increasing order
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n < 1, return 0 for safety */

    /* Determine the number of digits k needed for the nth value.
       For numbers with exactly k digits there are 2^k such values,
       and the cumulative count up to k digits is 2^(k+1) – 2. */
    idx_t k = 1;        /* start with one‑digit numbers */
    idx_t power = 2;    /* power = 2^k */

    /* Find the smallest k such that 2^(k+1) – 2 ≥ n. */
    while (1) {
        /* Prevent overflow of power << 1.  Once power is > 2^63
           we cannot generate more digits with an unsigned 64‑bit
           index anyway. */
        if (power > (UINT64_MAX >> 1))
            break;

        if ((power << 1) - 2 < n) {
            ++k;
            power <<= 1; /* now power = 2^(k+1) */
        } else
            break;
    }

    /* Index of the first k‑digit value (1‑based). */
    idx_t start = power - 1;
    /* Zero‑based offset within the block of k‑digit values. */
    idx_t r = n - start;

    /* Convert the offset r (interpreted as a binary number of k bits)
       into the required base‑13 digits: 0 → 1, 1 → 2. */
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i) {
        idx_t bit = (r >> (k - 1 - i)) & 1;
        val = val * 13 + (seq_t)(bit + 1);
    }
    return val;
}
