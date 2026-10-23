/*
 * A018747  Divisors of 964.
 * https://oeis.org/A018747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 964 (sorted ascending)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* There are exactly 6 divisors of 964 = 2^2 * 241^1:
       1, 2, 4, 241, 482, 964.
       We compute them once and return the n-th value. */
    static seq_t divs[6];
    static bool init = false;
    if (!init) {
        /* Compute 2^i for i=0..2 */
        seq_t pow2[3];
        pow2[0] = 1;
        pow2[1] = pow2[0] * (seq_t)2;
        pow2[2] = pow2[1] * (seq_t)2;
        /* Compute 241^j for j=0..1 */
        seq_t pow241[2];
        pow241[0] = 1;
        pow241[1] = pow241[0] * (seq_t)241;
        /* Generate all divisors */
        size_t idx = 0;
        for (size_t i = 0; i <= 2; ++i) {
            for (size_t j = 0; j <= 1; ++j) {
                divs[idx++] = pow2[i] * pow241[j];
            }
        }
        /* Simple insertion sort for 6 elements */
        for (size_t i = 1; i < 6; ++i) {
            seq_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }
        init = true;
    }
    if (n == 0 || n > 6) {
        /* Undefined for out-of-range n; return 0 as safe default */
        return (seq_t)0;
    }
    return divs[n - 1];
}
