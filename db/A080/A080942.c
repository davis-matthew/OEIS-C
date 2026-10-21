/*
 * A080942  Number of divisors of n that are also suffixes of n in binary
 *          representation.
 * https://oeis.org/A080942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors d of n such that the binary string of
 *            d occurs as a suffix of the binary string of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* number of bits needed to represent x, 0→0 */
static inline unsigned bitlen_u64(uint64_t x)
{
    return x == 0 ? 0 : 64 - __builtin_clzll(x);
}

/* increment count if d is a suffix divisor of n */
static inline void check_suffix(uint64_t n, uint64_t d, seq_t *cnt)
{
    unsigned len = bitlen_u64(d);
    uint64_t mask = (len == 64) ? ~0ULL : ((uint64_t)1 << len) - 1ULL;
    if ((n & mask) == d)
        (*cnt)++;
}

/* a(idx_t n) ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t res = 0;
    uint64_t limit = (uint64_t)n;
    uint64_t i = 1;
    for (; i * i <= limit; ++i)
    {
        if (limit % i == 0)
        {
            check_suffix(limit, i, &res);
            uint64_t j = limit / i;
            if (j != i)
                check_suffix(limit, j, &res);
        }
    }
    return res;
}
