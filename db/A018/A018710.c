/*
 * A018710  Divisors of 900.
 * https://oeis.org/A018710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 900 (sorted in ascending
 *            order)
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* All 27 divisors of 900 = 2^2 * 3^2 * 5^2 are generated once. */
    static seq_t divs[27];
    static int initialized = 0;

    if (!initialized) {
        int pos = 0;
        /* generate 2^i * 3^j * 5^k for i,j,k in {0,1,2} */
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                for (int k = 0; k < 3; ++k) {
                    seq_t v = 1;
                    for (int t = 0; t < i; ++t) v *= 2;
                    for (int t = 0; t < j; ++t) v *= 3;
                    for (int t = 0; t < k; ++t) v *= 5;
                    divs[pos++] = v;
                }
            }
        }

        /* simple selection sort – all values are ≤ 900, so comparisons are safe. */
        for (size_t i = 0; i < 27; ++i) {
            size_t min = i;
            for (size_t j = i + 1; j < 27; ++j)
                if (divs[j] < divs[min])
                    min = j;
            if (min != i) {
                seq_t tmp = divs[i];
                divs[i] = divs[min];
                divs[min] = tmp;
            }
        }
        initialized = 1;
    }

    if (n == 0 || n > 27) return 0;   /* out of range – conventionally 0 */
    return divs[n - 1];
}
