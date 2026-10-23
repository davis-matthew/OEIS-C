/*
 * A018358  Divisors of 255.
 * https://oeis.org/A018358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018358: nth divisor of 255 (sorted increasingly).
   The divisors of 255 are 1, 3, 5, 15, 17, 51, 85, 255. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* index starts at 1 */

    /* Factor 255 = 3 * 5 * 17 */
    const uint64_t target = 255;
    uint64_t tmp = target;
    uint64_t primes[3];
    int pc = 0;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            primes[pc++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        primes[pc++] = tmp;

    /* Enumerate all divisors via subset mask (exponents are 1) */
    uint64_t divs[16];
    int divc = 0;
    for (uint64_t mask = 0; mask < (1ULL << pc); ++mask)
    {
        uint64_t prod = 1;
        for (int i = 0; i < pc; ++i)
            if (mask & (1ULL << i))
                prod *= primes[i];
        divs[divc++] = prod;
    }

    /* Simple bubble sort (divc ≤ 8) */
    for (int i = 0; i < divc - 1; ++i)
        for (int j = i + 1; j < divc; ++j)
            if (divs[i] > divs[j])
            {
                uint64_t tmpv = divs[i];
                divs[i] = divs[j];
                divs[j] = tmpv;
            }

    if ((idx_t)(n - 1) < (idx_t)divc)
        return (seq_t)divs[n - 1];
    else
        return 0;            /* undefined for n > 8 */
}
