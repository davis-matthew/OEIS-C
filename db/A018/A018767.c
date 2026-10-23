/*
 * A018767  Divisors of 1000.
 * https://oeis.org/A018767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   compare_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 1000 sorted in ascending order (finite list of 16 terms). */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/compare_seq.h"

/* Comparator for qsort on seq_t. */

/* Cached list of divisors. */
static seq_t divisors[16];
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        int idx = 0;
        /* Exponents of prime factors 2 and 5 in 1000 = 2^3 * 5^3. */
        for (int e2 = 0; e2 <= 3; ++e2) {
            seq_t val2 = 1;
            for (int i = 0; i < e2; ++i) val2 *= (seq_t)2;
            for (int e5 = 0; e5 <= 3; ++e5) {
                seq_t val = val2;
                for (int j = 0; j < e5; ++j) val *= (seq_t)5;
                divisors[idx++] = val;
            }
        }
        /* Sort the divisors in ascending order. */
        qsort(divisors, 16, sizeof(seq_t), compare_seq);
        initialized = 1;
    }
    if (n == 0 || n > 16) {
        /* Undefined for indices outside the known range. */
        return 0;
    }
    return divisors[n - 1];
}
