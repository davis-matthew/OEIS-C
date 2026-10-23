/*
 * A106954  Primes of the form 4x^2+2xy+5y^2, with x and y any integer.
 * https://oeis.org/A106954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106954: primes of the form 4x² + 2xy + 5y² (x,y integers) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- primality test (deterministic Miller–Rabin for 64‑bit numbers) ---*/
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1)
            r = mod_mul_uint64(r, a, mod);
        a = mod_mul_uint64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Miller–Rabin with the deterministic bases for 64‑bit */
static bool isPrime64(uint64_t n)
{
    if (n < 2U) return false;
    /* small primes check */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i)
    {
        uint64_t p = small[i];
        if (n % p == 0ULL)
            return n == p;
    }

    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool ok = false;
        for (unsigned r = 1U; r < s; ++r)
        {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1)
            {
                ok = true;
                break;
            }
        }
        if (!ok) return false;
    }
    return true;
}

/*--- integer square root of a 128‑bit unsigned integer ---*/
static unsigned __int128 isqrt128(unsigned __int128 n)
{
    unsigned __int128 lo = 0, hi = (unsigned __int128)1 << 64;
    while (lo < hi)
    {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/*--- check whether p can be written as 4x²+2xy+5y² with (x,y) integers ---*/
static bool representable_uint64(uint64_t p)
{
    for (uint64_t y = 1ULL; ; ++y)
    {
        __int128 disc = (__int128)16 * (__int128)p - (__int128)76 * (__int128)y * (__int128)y;
        if (disc <= 0)
            break;                     /* no further solutions */
        unsigned __int128 udisc = (unsigned __int128)disc;
        unsigned __int128 s = isqrt128(udisc);
        if (s * s != udisc)
            continue;                  /* not a square */
        uint64_t S = (uint64_t)s;
        uint64_t t1 = (S >= 2ULL * y) ? (S - 2ULL * y) : (2ULL * y - S);
        if (t1 % 8ULL == 0ULL)
            return true;
        uint64_t t2 = S + 2ULL * y;
        if (t2 % 8ULL == 0ULL)
            return true;
    }
    return false;
}

/*--- main sequence function ---*/
seq_t a(idx_t n)
{
    if (n == 0ULL)
        return 0;   /* offset of the sequence is 1; return 0 for safety */

    static seq_t *arr  = NULL;
    static size_t cap  = 0;
    static size_t sz   = 0;
    static uint64_t cur = 2ULL;        /* start with the smallest prime */

    while (sz < n)
    {
        /* skip even numbers beyond 2 */
        if (cur > 2ULL && (cur & 1ULL) == 0ULL)
        {
            ++cur;
            continue;
        }

        if (isPrime64(cur) && representable_uint64(cur))
        {
            if (sz >= cap)
            {
                cap = (cap == 0) ? 16 : cap << 1;
                arr = (seq_t *)realloc(arr, cap * sizeof(seq_t));
            }
            arr[sz++] = (seq_t)cur;
        }
        ++cur;
    }
    return arr[n - 1];
}
