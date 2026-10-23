/*
 * A018291  Divisors of 116.
 * https://oeis.org/A018291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 116 in increasing order
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* constant whose divisors are listed */
    const idx_t target = 116;
    /* maximum possible number of divisors for this small number */
    seq_t divs[64];
    idx_t count = 0;

    /* enumerate divisors up to sqrt(target) */
    for (idx_t i = 1; i * i <= target; ++i) {
        if (target % i == 0) {
            divs[count++] = (seq_t)i;
            idx_t other = target / i;
            if (other != i) {
                divs[count++] = (seq_t)other;
            }
        }
    }

    /* insertion sort to get ascending order */
    for (idx_t i = 1; i < count; ++i) {
        seq_t key = divs[i];
        idx_t j = i;
        while (j > 0 && ((seq_t)divs[j - 1] > key)) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    /* n is 1-based; return the nth divisor if it exists */
    if (n > 0 && n <= count) {
        return divs[n - 1];
    }
    /* sequence has only 6 terms; out-of-range returns 0 */
    return (seq_t)0;
}
