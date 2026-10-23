/*
 * A018775  Divisors of 1014.
 * https://oeis.org/A018775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A018775: Divisors of 1014.
   a(n) is the n-th smallest positive divisor of 1014 (offset 1). */
#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t VAL = (idx_t)1014;
    seq_t divis[24];
    size_t count = 0;

    for (idx_t d = 1; d * d <= VAL; ++d) {
        if (VAL % d == 0) {
            divis[count++] = (seq_t)d;
            if (d * d != VAL) divis[count++] = (seq_t)(VAL / d);
        }
    }

    /* Sort the divisors in ascending order (insertion sort, trivial for ≤12 items). */
    for (size_t i = 1; i < count; ++i) {
        seq_t key = divis[i];
        size_t j = i;
        while (j > 0 && divis[j - 1] > key) {
            divis[j] = divis[j - 1];
            --j;
        }
        divis[j] = key;
    }

    if (n >= 1 && (size_t)(n - 1) < count) {
        return divis[(size_t)(n - 1)];
    } else {
        return (seq_t)0;
    }
}
