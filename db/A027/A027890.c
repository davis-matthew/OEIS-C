/*
 * A027890  Divisors of 99999999.
 * https://oeis.org/A027890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 99999999, sorted ascending. */
/* Sequence A027890: the set of all positive divisors of 99999999. */

/* This implementation generates the 48 divisors once,
   sorts them, and returns the n‑th term (1‑based).  For n > 48
   the function returns 0 as the sequence is finite. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq_t.h"   /* provides int cmp_seq_t(const void*,const void*) */

seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t divs[48];

    if (!init) {
        const idx_t primes[4] = {11, 73, 101, 137};
        const seq_t p3[3] = {1, 3, 9};
        idx_t cnt = 0;

        for (idx_t mask = 0; mask < ((idx_t)1 << 4); ++mask) {
            seq_t base = 1;
            for (idx_t i = 0; i < (idx_t)4; ++i) {
                if (mask & ((idx_t)1 << i))
                    base *= (seq_t)primes[i];
            }
            for (idx_t k = 0; k < (idx_t)3; ++k)
                divs[cnt++] = base * p3[k];
        }
        /* Sort the 48 divisors using the provided comparator. */
        qsort(divs, 48, sizeof(seq_t), cmp_seq_t);
        init = true;
    }

    if (n < 1 || n > (idx_t)48)
        return 0;
    return divs[(idx_t)n - 1];
}
