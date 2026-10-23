/*
 * A133780  Irregular array: n-th row lists the "non-isolated divisors" of
 *          (2n). A positive divisor, k, of n is non-isolated if (k-1) or
 *          (k+1) also divides n.
 * https://oeis.org/A133780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): the n‑th term of the concatenated rows of non‑isolated divisors of 2k.
   The sequence is computed by generating the rows one after the other,
   collecting the non‑isolated divisors in ascending order, and
   returning the target term. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t target = n;                /* index of the desired term (1‑based) */

    for (idx_t k = 1; ; ++k) {
        uint64_t m = 2ULL * k;       /* the integer whose divisors we examine */

        /* collect all positive divisors of m into a static array */
        uint64_t divs[512];
        size_t dcnt = 0;
        for (uint64_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                divs[dcnt++] = d;
                if (d != m / d) divs[dcnt++] = m / d;
            }
        }

        /* sort the divisors ascending (insertion sort, array size is tiny). */
        for (size_t i = 1; i < dcnt; ++i) {
            uint64_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }

        /* scan the sorted divisors and count the non‑isolated ones. */
        for (size_t idx = 0; idx < dcnt; ++idx) {
            uint64_t d = divs[idx];
            bool noniso = false;

            if (d > 1 && (m % (d - 1) == 0)) noniso = true;
            if (m % (d + 1) == 0)           noniso = true;

            if (noniso) {
                if (target == 1) return (seq_t)d;
                --target;
            }
        }
    }
}
