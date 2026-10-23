/*
 * A133030  Divisors of 5130.
 * https://oeis.org/A133030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n-th smallest divisor of 5130 (sorted ascending). */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Comparator for qsort on seq_t elements. */
static int seq_cmp(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    return (v1 > v2) - (v1 < v2);
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t divisors[32];
    static size_t count = 0;

    if (!initialized) {
        initialized = true;

        /* Precompute powers of 3 up to 3^3. */
        static const seq_t pow3[4] = {1, 3, 9, 27};

        size_t idx = 0;
        for (int a = 0; a <= 1; ++a) {              /* 2^a, a∈{0,1} */
            for (int b = 0; b <= 3; ++b) {          /* 3^b */
                for (int c = 0; c <= 1; ++c) {      /* 5^c */
                    for (int d = 0; d <= 1; ++d) {  /* 19^d */
                        seq_t val = (a ? (seq_t)2 : (seq_t)1);
                        val *= pow3[b];
                        val *= (c ? (seq_t)5 : (seq_t)1);
                        val *= (d ? (seq_t)19 : (seq_t)1);
                        divisors[idx++] = val;
                    }
                }
            }
        }
        count = idx;

        /* Sort the divisors in ascending order. */
        qsort(divisors, count, sizeof(seq_t), seq_cmp);
    }

    if (n == 0 || n > count)
        return (seq_t)0;   /* undefined for n outside the offset */
    return divisors[n - 1];
}
