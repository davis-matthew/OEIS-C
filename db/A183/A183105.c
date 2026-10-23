/*
 * A183105  a(n) = product of divisors of n that are not perfect powers.
 * https://oeis.org/A183105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = product of the divisors of n that are NOT perfect powers.
 *            The function enumerates all divisors of n, skips the
 *            perfect‑power ones, and multiplies the remaining ones together
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 59, u64 n <= 239,
 *            u128 n <= 839, big any n
 * Helpers:   _smallest_prime_factor, pow_uint64_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* External helper: returns the smallest prime factor of m,
   or 0 if m is prime.  The build supplies this header. */
#include "../utils/_smallest_prime_factor.h"
#include "../utils/pow_uint64_le.h"

/* Integer floor e‑th root of x, using binary search. */
static uint64_t int_root(uint64_t x, int e)
{
    uint64_t low = 1, high = x, best = 1;
    while (low <= high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        uint64_t p = pow_uint64_le(mid, e, x);
        if (p == x)
            return mid;
        if (p < x)
        {
            best = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return best;
}

/* Test whether x (>=2) is a perfect power m^k with k>=2. */
static bool is_perfect_power_u64(uint64_t x)
{
    if (x < 4)
        return false;                     /* 1,2,3 are not perfect powers */
    for (int e = 2; e <= 63; ++e)
    {
        if (pow_uint64_le(2, e, x) > x)
            break;                       /* no larger exponents possible */
        uint64_t r = int_root(x, e);
        if (r >= 2 && pow_uint64_le(r, e, x) == x)
            return true;
    }
    return false;
}

/* Recursive enumeration of all divisors of n, accumulating the product
   of those that are not perfect powers. */
static void dfs(uint64_t cur, size_t idx,
                const uint64_t *pr, const uint64_t *ex, size_t cnt,
                seq_t *prod)
{
    if (idx == cnt)
    {
        if (cur != 1 && !is_perfect_power_u64(cur))
            *prod = *prod * ((seq_t)cur);
        return;
    }
    uint64_t p = pr[idx];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e)
    {
        dfs(cur * val, idx + 1, pr, ex, cnt, prod);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* out of range, but make the function total */
        return 1;
    uint64_t m = (uint64_t)n;

    /* Factor n into primes and exponents.  `_smallest_prime_factor` works
       for 64‑bit integers. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    while (m > 1)
    {
        uint64_t p = _smallest_prime_factor(m);
        if (p == 0)           /* m itself is prime */
            p = m;
        uint64_t e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }
        primes[cnt] = p;
        exps[cnt]   = e;
        ++cnt;
    }

    /* Enumerate divisors and multiply the admissible ones. */
    seq_t prod = 1;
    dfs(1, 0, primes, exps, cnt, &prod);
    return prod;
}
