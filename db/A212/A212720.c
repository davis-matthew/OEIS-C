/*
 * A212720  The periodic part of the decimal expansion of n/(n+1). Any initial
 *          0's are to be placed at end of cycle.
 * https://oeis.org/A212720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 15, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the periodic part of the decimal expansion of n/(n+1).
   Any leading zeros in the cycle are moved to the end of the cycle. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* denominator */
    idx_t m = n + 1ULL;
    if (m <= 1ULL)   /* safety; n = 0 is not used in the OEIS entry */
        return 0;

    /* arrays of size m to record remainders and the resulting digits */
    size_t sz = (size_t)m;
    size_t *pos = (size_t *)calloc(sz, sizeof(size_t));
    if (!pos) return 0;
    int *digits = (int *)malloc(sz * sizeof(int));
    if (!digits) {
        free(pos);
        return 0;
    }

    size_t i = 0;
    size_t rem = (size_t)n;          /* current remainder, 0 < rem < m */

    seq_t result = 0;
    size_t cycle_start = 0, cycle_len = 0;

    /* generate decimal digits until a remainder repeats or becomes 0 */
    while (1) {
        if (rem == 0) {               /* terminating decimal */
            free(pos);
            free(digits);
            return 0;
        }
        if (pos[rem]) {               /* cycle detected */
            cycle_start = pos[rem] - 1;
            cycle_len   = i - cycle_start;
            break;
        }
        pos[rem] = i + 1;             /* first occurrence (index+1) */
        rem = rem * 10ULL;
        int digit = (int)(rem / m);
        digits[i] = digit;
        rem = rem % m;
        ++i;
    }

    /* rotate the cycle so that any leading zero digits are moved to the end */
    size_t offset = 0;
    for (size_t k = 0; k < cycle_len; ++k) {
        if (digits[cycle_start + k] != 0)
            break;
        ++offset;
    }

    for (size_t k = 0; k < cycle_len; ++k) {
        int d = digits[cycle_start + (offset + k) % cycle_len];
        result = result * 10ULL + (seq_t)d;
    }

    free(pos);
    free(digits);
    return result;
}
