/*
 * A178860  Divisors of 10080.
 * https://oeis.org/A178860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, easy, full
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 10080 in ascending order (a(1)=1, a(72)=10080) */
#include <stdlib.h>

static int _initialized = 0;
static seq_t _divisors[72];

static int _cmp_seq(const void *x, const void *y)
{
    const seq_t *a = (const seq_t *)x;
    const seq_t *b = (const seq_t *)y;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

static void _build_divisors(void)
{
    seq_t pow2[6];
    seq_t pow3[3];
    seq_t pow5[2];
    seq_t pow7[2];
    pow2[0] = 1;
    for (idx_t i = 1; i <= 5; ++i) pow2[i] = pow2[i-1] * (seq_t)2;
    pow3[0] = 1;
    for (idx_t i = 1; i <= 2; ++i) pow3[i] = pow3[i-1] * (seq_t)3;
    pow5[0] = 1; pow5[1] = 5;
    pow7[0] = 1; pow7[1] = 7;
    size_t idx = 0;
    for (idx_t e0 = 0; e0 <= 5; ++e0)
        for (idx_t e1 = 0; e1 <= 2; ++e1)
            for (idx_t e2 = 0; e2 <= 1; ++e2)
                for (idx_t e3 = 0; e3 <= 1; ++e3) {
                    _divisors[idx++] = pow2[e0] * pow3[e1] * pow5[e2] * pow7[e3];
                }
    qsort(_divisors, 72, sizeof(seq_t), _cmp_seq);
    _initialized = 1;
}

seq_t a(idx_t n)
{
    if (!_initialized) _build_divisors();
    if (n >= 1 && n <= 72) return _divisors[n-1];
    return 0;
}
