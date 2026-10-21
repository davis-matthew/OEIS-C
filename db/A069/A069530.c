/*
 * A069530  Smallest multiple of n with digit sum = 11, or 0 if no such number
 *          exists.
 * https://oeis.org/A069530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 0 if and only if n is divisible by 3 or by one of 1111,
 *            1507, 2849, 3367, 4849, 5291 or 7373; otherwise a(n) is the
 *            smallest multiple m = n*k such that the decimal digit sum of m
 *            equals 11
 * Range:     u8 n <= 9, u16 n <= 220, u32 n <= 2188, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses modular arithmetic to skip multiples that
   cannot have digit sum 11, and then searches the remaining multiples.
   For n divisible by any of the “bad” factors the search is skipped
   and 0 is returned. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* n is assumed >= 1 */

    /* n divisible by 3 -> impossible, because all multiples of a
       multiple of 3 have digit sum divisible by 3, never 11 */
    if (n % 3 == 0)
        return (seq_t)0;

    /* divisors that are known to produce no solution */
    static const idx_t bad_factors[] = {
        1111ULL, 1507ULL, 2849ULL, 3367ULL, 4849ULL, 5291ULL, 7373ULL
    };
    for (size_t i = 0; i < sizeof bad_factors / sizeof bad_factors[0]; ++i)
        if (n % bad_factors[i] == 0)
            return (seq_t)0;

    /* compute k0 such that n*k0 ≡ 2 (mod 9)  (digit sum ≡ 2 (mod 9))
       Since n is not divisible by 3, gcd(n,9)=1 to guarantee an inverse. */
    idx_t n9 = n % 9;          /* in {1,2,4,5,7,8} */
    static const idx_t inv9[9] = {0, 1, 5, 0, 7, 2, 0, 4, 8};
    idx_t k0 = (2 * inv9[n9]) % 9;   /* 1..8, 0 replaced by 9 */
    if (k0 == 0)
        k0 = 9;

    /* use a 64‑bit integer for the intermediate product – it is
       safely large enough for all expected n (≤ 10 000) and k. */
    unsigned long long product = (unsigned long long)n * (unsigned long long)k0;
    unsigned long long step    = (unsigned long long)n * 9ULL;

    for (;;)
    {
        /* compute decimal digit sum of product */
        unsigned long long temp = product;
        unsigned long long sum  = 0;
        while (temp > 0)
        {
            sum += temp % 10ULL;
            temp /= 10ULL;
        }
        if (sum == 11ULL)
            return (seq_t)product;

        product += step;
    }
}
