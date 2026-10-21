/*
 * A069526  Smallest multiple of n with digit sum = 7, or 0 if no such number
 *          exists, e.g. a(3k)= a(11k) = 0.
 * https://oeis.org/A069526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = first multiple of n whose digit sum is 7, or 0 if none
 *            exists. The function searches multiples of n that satisfy n*k ≡
 *            7 (mod 9) (necessary condition for a digit sum of 7) and returns
 *            the smallest such multiple. If no such multiple is found up to a
 *            fixed bound the function returns 0
 * Range:     u8 n <= 27, u16 n <= 111, u32 n <= 666, u64 n <= 9898,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fast rejection: if n divisible by 3 (or 9) no integer can have
       digit sum 7 and be a multiple of n. */
    if (n % 3 == 0)
        return (seq_t)0;

    /* Compute n modulo 9 (positive < 9). */
    int mod = (int)(n % 9);
    if (mod == 0)         /* should not happen because n % 3 == 0 is already handled */
        return (seq_t)0;

    /* Find modular inverse of n modulo 9. 9 and n are coprime because n % 3 != 0. */
    int inv = 1;
    for (int i = 1; i < 9; ++i)
    {
        if ((mod * i) % 9 == 1)
        {
            inv = i;
            break;
        }
    }

    /* Smallest k that makes n*k ≡ 7 (mod 9). */
    int k0 = (7 * inv) % 9;     /* 1 … 8 */
    uint64_t start_k = k0 ? (uint64_t)k0 : 9ULL;   /* never 0 but keep safe */

    /* Upper bound on k.  Even for n ≤ 10000, k ≤ 2,000,000 keeps
       n*k < 2^64 and offers a good chance to find a solution if it exists
       with moderate size.  */
    const uint64_t MAX_K = 2000000ULL;

    for (uint64_t k = start_k; k <= MAX_K; k += 9)
    {
        unsigned __int128 prod = (unsigned __int128)n * k;

        /* Compute digit sum of prod. */
        uint64_t s = 0;
        unsigned __int128 tmp = prod;
        while (tmp > 0)
        {
            s += (uint64_t)(tmp % 10);
            tmp /= 10;
        }

        if (s == 7)
            return (seq_t)prod;   /* prod fits in seq_t for the bound used */
    }

    /* No suitable multiple found within the bound. */
    return (seq_t)0;
}
