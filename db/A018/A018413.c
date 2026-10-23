/*
 * A018413  Divisors of 363.
 * https://oeis.org/A018413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: divisors of 363 sorted ascending.  a(n) = n‑th divisor of 363 (1 ≤ n ≤ 6). */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The divisor set: 363 = 3¹·11², divisors = {3^e1·11^e2 | e1∈{0,1}, e2∈{0,1,2}} */
    static seq_t divs[6];
    static bool built = false;

    if (!built) {
        /* generate all combinations of exponents */
        idx_t k = 0;
        for (idx_t e3 = 0; e3 <= 1; ++e3) {
            for (idx_t e11 = 0; e11 <= 2; ++e11) {
                seq_t val = (seq_t)1;
                /* power of 3 */
                for (idx_t i = 0; i < e3; ++i)
                    val *= (seq_t)3;
                /* power of 11 */
                for (idx_t i = 0; i < e11; ++i)
                    val *= (seq_t)11;
                divs[k++] = val;
            }
        }
        /* sort the 6 values ascending */
        for (idx_t i = 1; i < 6; ++i) {
            seq_t key = divs[i];
            idx_t j = i;
            while (j > 0 && divs[j-1] > key) {
                divs[j] = divs[j-1];
                --j;
            }
            divs[j] = key;
        }
        built = true;
    }

    return (n >= 1 && n <= 6) ? divs[n-1] : (seq_t)0;
}
