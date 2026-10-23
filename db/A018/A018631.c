/*
 * A018631  Divisors of 760.
 * https://oeis.org/A018631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 760 in ascending order
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Finite sequence of length 16) */

#include <stdbool.h>           /* for bool */

seq_t a(idx_t n)
{
    /* Static cache of the 16 divisors of 760 = 2^3 * 5 * 19. */
    static seq_t divisors[16];    /* 4 · 2 · 2 = 16 combinations */
    static bool ready = false;    /* set after first time building */

    if (!ready) {
        /* Generate all divisor candidates by exponent loops. */
        idx_t idx = 0;
        for (idx_t e2 = 0; e2 <= 3; ++e2) {      /* exponent of 2 */
            for (idx_t e5 = 0; e5 <= 1; ++e5) {  /* exponent of 5 */
                for (idx_t e19 = 0; e19 <= 1; ++e19) { /* exponent of 19 */
                    seq_t val = 1;
                    for (idx_t k = 0; k < e2; ++k)  val *= 2;
                    for (idx_t k = 0; k < e5; ++k)  val *= 5;
                    for (idx_t k = 0; k < e19; ++k) val *= 19;
                    divisors[idx++] = val;
                }
            }
        }
        /* Simple bubble sort to put the 16 divisors in ascending order. */
        for (idx_t i = 0; i < 16; ++i) {
            for (idx_t j = i + 1; j < 16; ++j) {
                if (divisors[j] < divisors[i]) {
                    seq_t tmp = divisors[i];
                    divisors[i] = divisors[j];
                    divisors[j] = tmp;
                }
            }
        }
        ready = true;
    }

    if (n == 0 || n > 16) return 0;   /* undefined for out‑of‑range n */
    return divisors[n - 1];
}
