/*
 * A018416  Divisors of 368.
 * https://oeis.org/A018416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 368 (sorted ascending) */
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[10];
    static size_t init = 0;
    if (!init) {
        size_t k = 0;
        /* Generate 2^a for a = 0..4 */
        for (int a = 0; a <= 4; ++a) {
            divs[k++] = ((seq_t)1 << a);
        }
        /* Generate 23 * 2^a for a = 0..4 */
        for (int a = 0; a <= 4; ++a) {
            divs[k++] = ((seq_t)23) * ((seq_t)1 << a);
        }
        init = 1;
    }
    if (n == 0) return 0;
    if (n <= 10) return divs[n - 1];
    return 0;
}
