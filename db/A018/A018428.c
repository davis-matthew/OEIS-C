/*
 * A018428  Divisors of 388.
 * https://oeis.org/A018428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n = 1,2,4,97,194,388 (divisors of 388)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

static seq_t divisors[6];
static idx_t num_divisors = 0;

static void init_divisors(void)
{
    if (num_divisors != 0) return;  /* already initialized */

    /* Prime factorization of 388: 2^2 * 97^1 */
    const seq_t prime[2] = { (seq_t)2, (seq_t)97 };
    const idx_t expmax[2] = { 2, 1 };

    size_t idx = 0;
    for (idx_t e0 = 0; e0 <= expmax[0]; ++e0)
    {
        seq_t p0 = seq_pow(prime[0], e0);
        for (idx_t e1 = 0; e1 <= expmax[1]; ++e1)
        {
            seq_t p1 = seq_pow(prime[1], e1);
            divisors[idx++] = seq_mul(p0, p1);
        }
    }

    /* Simple insertion sort for the 6 elements */
    for (size_t i = 1; i < 6; ++i)
    {
        seq_t key = divisors[i];
        size_t j = i;
        while (j > 0 && divisors[j - 1] > key)
        {
            divisors[j] = divisors[j - 1];
            --j;
        }
        divisors[j] = key;
    }

    num_divisors = 6;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    init_divisors();
    if (n <= num_divisors) return divisors[n - 1];
    return (seq_t)0;
}
