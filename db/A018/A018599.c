/*
 * A018599  Divisors of 702.
 * https://oeis.org/A018599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th smallest divisor of 702 (sorted ascending)
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const seq_t N = 702;          /* target number */
    seq_t vals[256];
    size_t cnt = 0;

    for (idx_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            vals[cnt++] = i;
            if (i != N / i) {
                vals[cnt++] = N / i;
            }
        }
    }
    /* insertion sort – the count is 16, so this is trivial */
    for (size_t i = 1; i < cnt; ++i) {
        seq_t key = vals[i];
        size_t j = i;
        while (j > 0 && vals[j-1] > key) {
            vals[j] = vals[j-1];
            --j;
        }
        vals[j] = key;
    }
    if (n >= 1 && (idx_t)n <= cnt) {
        return vals[n - 1];
    } else {
        return 0;  /* undefined for n > number of divisors */
    }
}
