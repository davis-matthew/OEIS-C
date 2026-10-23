/*
 * A178878  Divisors of 1680.
 * https://oeis.org/A178878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 1680 sorted in increasing order (a(1)=1, a(40)=1680) */
#include <stdlib.h>
#include <stdbool.h>

/* Compare two seq_t values for qsort (works in C and C++) */
static int seq_t_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* There are 5*2*2*2 = 40 divisors of 1680. */
    static seq_t divisors[40];
    static bool init = false;

    if (!init) {
        size_t idx = 0;
        /* 1680 = 2^4 * 3^1 * 5^1 * 7^1 */
        for (idx_t e2 = 0; e2 <= 4; ++e2) {
            seq_t val2 = 1;
            for (idx_t i = 0; i < e2; ++i) val2 *= (seq_t)2;
            for (idx_t e3 = 0; e3 <= 1; ++e3) {
                seq_t val3 = val2;
                if (e3) val3 *= (seq_t)3;
                for (idx_t e5 = 0; e5 <= 1; ++e5) {
                    seq_t val5 = val3;
                    if (e5) val5 *= (seq_t)5;
                    for (idx_t e7 = 0; e7 <= 1; ++e7) {
                        seq_t val7 = val5;
                        if (e7) val7 *= (seq_t)7;
                        divisors[idx++] = val7;
                    }
                }
            }
        }
        /* Sort the 40 divisor values. */
        qsort(divisors, 40, sizeof(seq_t), seq_t_cmp);
        init = true;
    }

    if (n < 1 || n > 40) return (seq_t)0;
    return divisors[n - 1];
}
