/*
 * A018339  Divisors of 220.
 * https://oeis.org/A018339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 220 (sorted ascending)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t divisors[12];
    static bool initialized = false;

    if (!initialized) {
        size_t idx = 0;
        for (idx_t a_exp = 0; a_exp <= 2; ++a_exp) {
            seq_t val = (seq_t)1 << a_exp;
            for (idx_t b_exp = 0; b_exp <= 1; ++b_exp) {
                seq_t valb = val;
                if (b_exp == 1)
                    valb *= (seq_t)5;
                for (idx_t c_exp = 0; c_exp <= 1; ++c_exp) {
                    seq_t valc = valb;
                    if (c_exp == 1)
                        valc *= (seq_t)11;
                    divisors[idx++] = valc;
                }
            }
        }
        qsort(divisors, 12, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n - 1 < 12)
        return divisors[n - 1];
    else
        return (seq_t)0; /* index beyond available divisors */
}
