/*
 * A018779  Divisors of 1020.
 * https://oeis.org/A018779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 1020 in ascending order. */
#include <stddef.h>

/* Generate all divisors of 1020 (2^2 * 3 * 5 * 17) once. */
static seq_t divisors[24];
static int divisors_ready = 0;

/* Simple insertion sort for 24 elements, removes dependency on qsort. */
static void sort_divisors(void) {
    size_t i, j;
    seq_t key;
    for (i = 1; i < 24; ++i) {
        key = divisors[i];
        j = i;
        while (j > 0 && divisors[j - 1] > key) {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }
}

seq_t a(idx_t n)
{
    /* Lazily compute the list of divisors once. */
    if (!divisors_ready) {
        size_t pos = 0;
        seq_t v2 = 1;
        for (int e2 = 0; e2 <= 2; ++e2) {
            if (e2 > 0) v2 *= 2;
            seq_t v3 = v2;
            for (int e3 = 0; e3 <= 1; ++e3) {
                if (e3 > 0) v3 *= 3;
                seq_t v5 = v3;
                for (int e5 = 0; e5 <= 1; ++e5) {
                    if (e5 > 0) v5 *= 5;
                    seq_t v17 = v5;
                    for (int e17 = 0; e17 <= 1; ++e17) {
                        if (e17 > 0) v17 *= 17;
                        divisors[pos++] = v17;
                    }
                }
            }
        }
        sort_divisors();
        divisors_ready = 1;
    }
    if (n < 1 || n > 24) return (seq_t)0;   /* undefined beyond sequence length */
    return divisors[(size_t)(n - 1)];
}
