/*
 * A018774  Divisors of 1012.
 * https://oeis.org/A018774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th divisor of 1012, sorted ascending
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

seq_t a(idx_t n)
{
    /* Divisors of 1012 in increasing order */
    static const seq_t divs[] = {
        (seq_t)1,
        (seq_t)2,
        (seq_t)4,
        (seq_t)11,
        (seq_t)22,
        (seq_t)23,
        (seq_t)44,
        (seq_t)46,
        (seq_t)92,
        (seq_t)253,
        (seq_t)506,
        (seq_t)1012
    };
    static const size_t cnt = sizeof(divs)/sizeof(divs[0]);

    if (n == 0) return 0;          /* guard against the offset being 0 */
    if (n-1 < cnt) return divs[n-1];
    return 0;                      /* undefined for n > 12 */
}
