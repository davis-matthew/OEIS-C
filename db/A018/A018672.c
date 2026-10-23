/*
 * A018672  Divisors of 833.
 * https://oeis.org/A018672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th divisor of 833 in ascending order (offset 1). */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle requests below the offset.  Behaviour here is irrelevant. */
    if (n == 0) return (seq_t)0;

    /* Divisors of 833: 1, 7, 17, 49, 119, 833.  
       We generate them once and cache in a static array. */
    static seq_t divs[6];
    static size_t div_cnt = 0;

    if (div_cnt == 0) {
        /* Compute all divisors of 833. */
        const seq_t N = (seq_t)833;
        for (idx_t i = 1; i <= (idx_t)833; ++i) {
            if (N % (seq_t)i == (seq_t)0) {
                if (div_cnt < 6) divs[div_cnt++] = (seq_t)i;
            }
        }
    }

    /* Return the nth divisor if it exists, otherwise 0. */
    if ((idx_t)div_cnt < n) return (seq_t)0;
    return divs[n - 1];
}
