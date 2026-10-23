/*
 * A018601  Divisors of 705.
 * https://oeis.org/A018601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th smallest divisor of 705 (1-indexed)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t primes[3] = { (seq_t)3, (seq_t)5, (seq_t)47 };

seq_t a(idx_t n)
{
    /* 705 = 3·5·47, so it has 8 divisors.
       Generate all products of the form 3^e0·5^e1·47^e2 with ei∈{0,1}. */
    seq_t vals[8];
    idx_t k = 0;
    for (idx_t e0 = 0; e0 < 2; ++e0)
    for (idx_t e1 = 0; e1 < 2; ++e1)
    for (idx_t e2 = 0; e2 < 2; ++e2)
    {
        seq_t v = (seq_t)1;
        if (e0) v *= primes[0];
        if (e1) v *= primes[1];
        if (e2) v *= primes[2];
        vals[k++] = v;
    }

    /* insertion sort the 8 values (array size is fixed) */
    for (idx_t i = 1; i < 8; ++i)
    {
        seq_t key = vals[i];
        idx_t j = i;
        while (j > 0 && vals[j - 1] > key)
        {
            vals[j] = vals[j - 1];
            --j;
        }
        vals[j] = key;
    }

    if (n >= 1 && n <= 8)
        return vals[n - 1];
    else
        return (seq_t)0;
}
