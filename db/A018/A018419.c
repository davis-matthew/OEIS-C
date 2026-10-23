/*
 * A018419  Divisors of 372.
 * https://oeis.org/A018419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the nth divisor of 372 in increasing order
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The divisors of 372 are: 1, 2, 3, 4, 6, 12, 31, 62, 93, 124, 186, 372. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static array to hold all divisors of 372, sorted. */
    static seq_t divisors[12];
    static int initialized = 0;
    if (!initialized)
    {
        /* Prime factorisation 372 = 2^2 * 3^1 * 31^1 */
        const unsigned primes[] = {2, 3, 31};
        const unsigned exps[]  = {2, 1, 1};
        const size_t pcount = 3;
        size_t idx = 0;
        for (unsigned i = 0; i <= exps[0]; ++i)
        {
            for (unsigned j = 0; j <= exps[1]; ++j)
            {
                for (unsigned k = 0; k <= exps[2]; ++k)
                {
                    seq_t val = 1;
                    for (unsigned e = 0; e < i; ++e) val *= (seq_t)primes[0];
                    for (unsigned e = 0; e < j; ++e) val *= (seq_t)primes[1];
                    for (unsigned e = 0; e < k; ++e) val *= (seq_t)primes[2];
                    divisors[idx++] = val;
                }
            }
        }
        /* Sort the array (simple selection sort, 12 elements only). */
        for (size_t i = 0; i < 12; ++i)
        {
            size_t minidx = i;
            for (size_t j = i + 1; j < 12; ++j)
            {
                if (divisors[j] < divisors[minidx])
                    minidx = j;
            }
            if (minidx != i)
            {
                seq_t tmp = divisors[i];
                divisors[i] = divisors[minidx];
                divisors[minidx] = tmp;
            }
        }
        initialized = 1;
    }
    if (n == 0)  /* offset is 1, but guard against 0 */
        return 0;
    if (n <= 12)
        return divisors[(size_t)(n - 1)];
    /* Sequence is finite; return 0 for out-of-bounds indices. */
    return 0;
}
