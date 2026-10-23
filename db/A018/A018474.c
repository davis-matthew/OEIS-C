/*
 * A018474  Divisors of 474.
 * https://oeis.org/A018474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Divisors of 474 – a finite sequence. 
    Returns the n-th smallest positive divisor of 474;
    if n exceeds the number of divisors, returns 0. */
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t divisors[64];
    static size_t dcnt = 0;

    if (dcnt == 0) {
        const idx_t N = 474;
        /* Generate all divisors of 474 */
        for (idx_t d = 1; d <= N; ++d) {
            if (N % d == 0) {
                divisors[dcnt++] = (seq_t)d;
            }
        }
        /* Simple insertion sort (small number of divisors) */
        for (size_t i = 0; i < dcnt; ++i) {
            for (size_t j = i + 1; j < dcnt; ++j) {
                if (divisors[i] > divisors[j]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
    }

    if (n <= dcnt) {
        return divisors[n - 1];
    } else {
        return (seq_t)0;
    }
}
