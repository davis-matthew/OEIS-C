/*
 * A018572  Divisors of 651.
 * https://oeis.org/A018572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   generate the n‑th divisor of 651 by enumerating its prime
 *            factors
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   cmp_func
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_func.h"

/* sequence a(n): divisors of 651, sorted ascending */
seq_t a(idx_t n)
{
    /* static cache of the 8 divisors */
    static seq_t divs[8];
    static bool init = false;

    if (!init) {                    /* compute once on first call */
        seq_t temp[8];
        size_t pos = 0;

        for (int e1 = 0; e1 <= 1; ++e1)
            for (int e2 = 0; e2 <= 1; ++e2)
                for (int e3 = 0; e3 <= 1; ++e3) {
                    seq_t val = 1;
                    if (e1) val *= 3;
                    if (e2) val *= 7;
                    if (e3) val *= 31;
                    temp[pos++] = val;
                }

        qsort(temp, 8, sizeof(seq_t), cmp_func);
        for (size_t i = 0; i < 8; ++i) divs[i] = temp[i];
        init = true;
    }

    if (n == 0) return 0;            /* below offset: undefined, return 0 */
    if (n <= 8) return divs[n - 1];   /* a(1) -> divs[0] */
    return 0;                        /* values beyond the last divisor are 0 */
}
