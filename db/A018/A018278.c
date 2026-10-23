/*
 * A018278  Divisors of 90.
 * https://oeis.org/A018278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the nth divisor of 90 in ascending order. The sequence
 *            is finite (12 terms)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Divisors of 90 generated from its prime factorisation 2^1 * 3^2 * 5^1 */
    static seq_t divs[12];
    static int initialized = 0;

    if (!initialized) {
        idx_t pos = 0;
        seq_t v;

        for (idx_t e2 = 0; e2 <= 1; e2++) {
            for (idx_t e3 = 0; e3 <= 2; e3++) {
                for (idx_t e5 = 0; e5 <= 1; e5++) {
                    v = (seq_t)1;
                    for (idx_t i = 0; i < e2; i++) v *= 2;
                    for (idx_t i = 0; i < e3; i++) v *= 3;
                    for (idx_t i = 0; i < e5; i++) v *= 5;
                    divs[pos++] = v;
                }
            }
        }

        /* Sort the 12 values in ascending order */
        for (size_t i = 0; i < 12; i++) {
            for (size_t j = i + 1; j < 12; j++) {
                if (divs[i] > divs[j]) {
                    seq_t tmp = divs[i];
                    divs[i] = divs[j];
                    divs[j] = tmp;
                }
            }
        }
        initialized = 1;
    }

    if (n == 0) return 0;          /* offset starts at 1; n>=1 is normal usage */
    if (n <= 12) return divs[n - 1];
    return 0;                      /* undefined beyond known terms */
}
