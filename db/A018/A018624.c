/*
 * A018624  Divisors of 747.
 * https://oeis.org/A018624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 747 (0 for n > 6)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 747 = 3^2 * 83, it has 6 divisors */
    const seq_t val = (seq_t)747;
    const size_t max_divs = 6;
    seq_t divs[max_divs];
    size_t cnt = 0;

    /* Collect all divisors */
    for (idx_t i = 1; (seq_t)i * (seq_t)i <= val; ++i) {
        if (val % (seq_t)i == 0) {
            /* i is a divisor */
            divs[cnt++] = (seq_t)i;
            if (cnt == max_divs) break;
            /* val / i is another divisor unless equal */
            seq_t d = val / (seq_t)i;
            if (d != (seq_t)i && cnt < max_divs) {
                divs[cnt++] = d;
                if (cnt == max_divs) break;
            }
        }
    }

    /* Simple insertion sort (array size <= 6) */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = divs[i];
        size_t j = i;
        while (j > 0 && divs[j - 1] > key) {
            divs[j] = divs[j - 1];
            --j;
        }
        divs[j] = key;
    }

    if (n == 0 || n > cnt) return (seq_t)0;
    return divs[n - 1];
}
