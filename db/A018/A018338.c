/*
 * A018338  Divisors of 216.
 * https://oeis.org/A018338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth smallest divisor of 216 (0 if n exceeds number of
 *            divisors)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort on seq_t */
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
    /* Divisors of 216 (2^3 * 3^3) – 16 total */
    static seq_t divs[16];
    static size_t ndiv = 0;

    if (ndiv == 0) {
        /* Generate all 16 divisors unsorted */
        size_t idx = 0;
        seq_t pow3[4];
        pow3[0] = (seq_t)1;
        for (int e = 1; e <= 3; ++e)
            pow3[e] = pow3[e - 1] * (seq_t)3;

        for (int i = 0; i <= 3; ++i) {
            seq_t val2 = (seq_t)1 << i;   /* 2^i */
            for (int j = 0; j <= 3; ++j) {
                divs[idx++] = val2 * pow3[j];
            }
        }
        /* Sort in ascending order */
        qsort(divs, 16, sizeof(seq_t), cmp_seq_t);
        ndiv = 16;
    }

    if (n == 0 || n > ndiv) return (seq_t)0;   /* Undefined index returns 0 */
    return divs[(size_t)(n - 1)];
}
