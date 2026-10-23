/*
 * A018259  Divisors of 44.
 * https://oeis.org/A018259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018259: divisors of 44 in increasing order */
#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    const idx_t N = 44;            /* the number whose divisors we list */
    static seq_t divs[16];         /* enough space for all divisors of 44 */
    static idx_t cnt = 0;          /* number of divisors found so far */

    if (cnt == 0) {               /* build the divisor table once */
        for (idx_t d = 1; d <= N; ++d) {
            if (N % d == 0) {
                divs[cnt++] = (seq_t)d;
            }
        }
    }

    if (n < 1 || n > cnt) return 0;             /* undefined for out‑of‑range n */
    return divs[(size_t)(n - 1)];               /* zero‑based array access */
}
