/*
 * A198343  Divisors of 196560.
 * https://oeis.org/A198343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n-th divisor of 196560 (sorted in ascending order)
 * Range:     u8 n <= 4, u16 n <= 154, u32 n <= 160, u64 n <= 160,
 *            u128 n <= 160, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Precomputed number of divisors of 196560 (5*4*2*2*2 = 160) */
#define DIVISOR_COUNT 160

/* Power tables for the prime factorization 196560 = 2^4 * 3^3 * 5^1 * 7^1 * 13^1 */
static const seq_t pow2[5]  = {1,2,4,8,16};
static const seq_t pow3[4]  = {1,3,9,27};
static const seq_t pow5[2]  = {1,5};
static const seq_t pow7[2]  = {1,7};
static const seq_t pow13[2] = {1,13};

/* Comparator for qsort */
static int seq_compare(const void *p, const void *q)
{
    seq_t a = *(const seq_t *)p;
    seq_t b = *(const seq_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

static seq_t divisors[DIVISOR_COUNT];
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Generate all divisors */
        idx_t idx = 0;
        for (idx_t a = 0; a < 5; ++a)
            for (idx_t b = 0; b < 4; ++b)
                for (idx_t c = 0; c < 2; ++c)
                    for (idx_t d = 0; d < 2; ++d)
                        for (idx_t e = 0; e < 2; ++e) {
                            seq_t val = pow2[a] * pow3[b] * pow5[c] * pow7[d] * pow13[e];
                            divisors[idx++] = val;
                        }
        /* Sort divisors */
        qsort(divisors, DIVISOR_COUNT, sizeof(seq_t), seq_compare);
        initialized = 1;
    }

    if (n == 0 || n > DIVISOR_COUNT)
        return 0; /* Undefined for n outside the range */

    return divisors[n - 1];
}
