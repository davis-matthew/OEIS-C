/*
 * A018466  Divisors of 459.
 * https://oeis.org/A018466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 459: a(n) = the n-th divisor of 459 in increasing order. Computed by enumerating prime power combinations. */
#include <stdlib.h>

static seq_t divisors[8];
static int initialized = 0;

static int seq_cmp(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t*)p1;
    seq_t b = *(const seq_t*)p2;
    return (a < b) ? -1 : (a > b);
}

static void init_divisors(void)
{
    if (initialized) return;
    seq_t pow3[4];
    pow3[0] = 1;
    for (unsigned u = 1; u < 4; ++u) pow3[u] = pow3[u - 1] * (seq_t)3;
    seq_t pow17[2];
    pow17[0] = 1;
    pow17[1] = (seq_t)17;

    unsigned idx = 0;
    for (unsigned e3 = 0; e3 < 4; ++e3) {
        for (unsigned e17 = 0; e17 < 2; ++e17) {
            divisors[idx++] = pow3[e3] * pow17[e17];
        }
    }
    qsort(divisors, 8, sizeof(seq_t), seq_cmp);
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    init_divisors();
    if (n <= 8) {
        return divisors[n - 1];
    }
    return 0;
}
