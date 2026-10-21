/*
 * A095049  Number of distinct digits needed to write in decimal
 *          representation all positive non-divisors of n that are less than
 *          n.
 * https://oeis.org/A095049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10 for n > 20; otherwise count distinct decimal digits of
 *            * all positive integers < n that do not divide n. * The returned
 *            value is the number of distinct digits (0‑9) that appear in *
 *            the decimal representations of those non‑divisors. For n > 20
 *            the set * of digits always contains all ten...
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* Fast path for the trivial case */
    if (n > 20u)
        return (seq_t)10u;

    /* Bitmask of digits that have been seen: bit i corresponds to digit i. */
    uint16_t mask = 0;

    /* Enumerate all numbers less than n that are non‑divisors of n. */
    for (idx_t k = 1; k < n; ++k) {
        if (n % k == 0)          /* k is a divisor ; skip it */
            continue;

        /* Extract decimal digits of k and record them. */
        idx_t temp = k;
        do {
            mask |= (1u << (temp % 10u));
            temp /= 10u;
        } while (temp > 0u);
    }

    /* Count how many distinct digits were seen.  There are at most 10 bits. */
    unsigned int count = 0;
    for (unsigned int d = 0; d < 10u; ++d)
        if (mask & (1u << d))
            ++count;

    return (seq_t)count;
}
