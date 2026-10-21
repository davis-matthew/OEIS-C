/*
 * A081326  Number of partitions of n into two 3-smooth numbers.
 * https://oeis.org/A081326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered partitions of n into two 3‑smooth
 *            numbers (numbers whose prime factors are only 2 or 3)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* No partitions for n < 2 */
    if (n < 2)
        return (seq_t)0;

    /* Allocate a bit‑mask array to mark 3‑smooth numbers up to n. */
    char *is_smooth = (char *)malloc((size_t)(n + 1));
    if (!is_smooth)
        return (seq_t)0; /* Allocation failure; return 0 as safety. */

    /* Initialise the array to zero. */
    for (idx_t i = 0; i <= n; ++i)
        is_smooth[i] = 0;

    /* First pass: mark all 3‑smooth numbers ≤ n. */
    for (idx_t val2 = 1; val2 <= n; val2 <<= 1) {
        for (idx_t val3 = 1; ; val3 *= 3) {
            if (val3 > n / val2)   /* prevent overflow and exceed n */
                break;
            idx_t val = val2 * val3;
            is_smooth[val] = 1;
        }
    }

    /* Second pass: count unordered pairs (a, n‑a) with a ≤ n/2. */
    seq_t count = 0;
    idx_t half = n / 2;
    for (idx_t val2 = 1; val2 <= half; val2 <<= 1) {
        for (idx_t val3 = 1; ; val3 *= 3) {
            if (val3 > half / val2)
                break;
            idx_t v = val2 * val3;
            if (v > half)
                break;
            if (is_smooth[n - v])
                ++count;
        }
    }

    free(is_smooth);
    return count;
}
