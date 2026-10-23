/*
 * A018629  Divisors of 756.
 * https://oeis.org/A018629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) is the n-th smallest divisor of 756 (sorted ascending)
 * Range:     u8 n <= 19, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A018629 – Divisors of 756 */

#include <stdbool.h>
#include <stdlib.h>

/* Comparator compatible with C and C++ for seq_t. */
static int seq_compare(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    return (*a > *b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static seq_t divisors[24];
    static bool initialized = false;

    if (!initialized) {
        size_t idx = 0;

        /* Enumerate all combinations of exponents for 2^2 * 3^3 * 7^1. */
        for (int e2 = 0; e2 <= 2; ++e2) {
            seq_t pow2 = 1;
            for (int i = 0; i < e2; ++i) pow2 *= (seq_t)2;

            for (int e3 = 0; e3 <= 3; ++e3) {
                seq_t pow3 = pow2;
                for (int i = 0; i < e3; ++i) pow3 *= (seq_t)3;

                for (int e7 = 0; e7 <= 1; ++e7) {
                    seq_t val = pow3;
                    if (e7) val *= (seq_t)7;
                    divisors[idx++] = val;
                }
            }
        }

        /* Sort the generated divisors. */
        qsort(divisors, 24, sizeof(seq_t), seq_compare);
        initialized = true;
    }

    return (n >= 1 && n <= 24) ? divisors[n - 1] : (seq_t)0;
}
