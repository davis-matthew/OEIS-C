/*
 * A096377  Floor of area of triangle with consecutive prime sides.
 * https://oeis.org/A096377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(4*p_n^2*p_{n+2}^2 - (p_n^2 - p_{n+1}^2 +
 *            p_{n+2}^2)^2) / 4)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root of an unsigned __int128 value. */
static uint64_t isqrt128(unsigned __int128 n)
{
    uint64_t low = 0;
    uint64_t high = 1;
    while ((unsigned __int128)high * high <= n)
    {
        high <<= 1;
        if (high == 0)
        {
            high = UINT64_MAX;
            break;
        }
    }
    while (low < high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Prime list – maintained up to the required index. */
static uint64_t *prime_arr = NULL;
static size_t prime_len  = 0;
static size_t prime_cap  = 0;
static idx_t  prime_limit = 0;

static void ensure_primes(idx_t need)
{
    if (prime_len >= need)
        return;

    idx_t limit = prime_limit ? prime_limit : 100;
    while (prime_len < need)
    {
        limit = limit * 2 + 10;

        free(prime_arr);
        prime_arr = NULL;
        prime_len = 0;
        prime_cap = 0;

        /* Allocate composite flag array. */
        uint8_t *isComp = (uint8_t *)calloc(limit + 1, 1);
        if (!isComp)
            exit(1);

        for (idx_t i = 2; i <= limit; ++i)
        {
            if (!isComp[i])
            {
                if (prime_len >= prime_cap)
                {
                    prime_cap = prime_cap ? prime_cap << 1 : 64;
                    uint64_t *tmp = (uint64_t *)realloc(prime_arr, prime_cap * sizeof(uint64_t));
                    if (!tmp)
                        exit(1);
                    prime_arr = tmp;
                }
                prime_arr[prime_len++] = i;

                if (i <= limit / i)
                {
                    for (idx_t j = i * i; j <= limit; j += i)
                        isComp[j] = 1;
                }
            }
        }

        free(isComp);
        prime_limit = limit;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_primes(n + 2);

    uint64_t pa = prime_arr[n - 1];
    uint64_t pb = prime_arr[n];
    uint64_t pc = prime_arr[n + 1];

    unsigned __int128 a_sq = (unsigned __int128)pa * pa;
    unsigned __int128 b_sq = (unsigned __int128)pb * pb;
    unsigned __int128 c_sq = (unsigned __int128)pc * pc;

    unsigned __int128 term1 = 4 * a_sq * c_sq;

    __int128 tmp = (__int128)a_sq - (__int128)b_sq + (__int128)c_sq;
    unsigned __int128 tmp_sq = (unsigned __int128)tmp * tmp;

    unsigned __int128 D = term1 - tmp_sq;

    uint64_t sqrtD = isqrt128(D);
    return (seq_t)(sqrtD / 4);
}
