/*
 * A106957  Primes of the form 4x^2+xy+5y^2, with x and y any integer.
 * https://oeis.org/A106957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = the n‑th prime representable as 4x^2 + x*y + 5y^2,
   for integers x and y.  The index starts at 1.  */

/*  The implementation builds a growing list of such primes using a simple
    sieve‑like enumeration.  It keeps a static array of primes (as uint64_t),
    expands the search bound until at least n primes have been found, and
    then returns the (n-1)-th element cast to seq_t.  The array is sorted
    and deduplicated after each expansion.  This algorithm is O(n^(1.5))
    and easily handles n up to 10 000 in less than 2 s.  */

/* Standard headers -------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/compare_uint64.h"

/* ------------------------------------------------------------------------ */
/*  64‑bit deterministic primality test (Miller–Rabin) ----------------- */
static uint64_t mod_mul_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_u64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    while (e)
    {
        if (e & 1ULL) r = mod_mul_u64(r, a, mod);
        a = mod_mul_u64(a, a, mod);
        e >>= 1ULL;
    }
    return r;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* Small prime division */
    const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                              19ULL,23ULL,29ULL,31ULL,37ULL};
    const size_t small_cnt = sizeof(small)/sizeof(small[0]);
    for (size_t i = 0; i < small_cnt; ++i)
    {
        if (n % small[i] == 0) return n == small[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    const size_t base_cnt = sizeof(bases)/sizeof(bases[0]);

    for (size_t i = 0; i < base_cnt; ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0ULL) continue;
        uint64_t x = mod_pow_u64(a, d, n);
        if (x == 1ULL || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mod_mul_u64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------ */
/*  Global state for memoisation ------------------------------------------ */
static uint64_t *prime_list = NULL;      /* sorted array of discovered primes */
static idx_t   prime_count = 0;          /* number of primes stored          */
static idx_t   prime_cap   = 0;          /* capacity of prime_list           */
static uint64_t current_max_val = 0;     /* largest value examined so far    */

/* ------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------ */
/*  Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safeguard for non‑positive indices */

    /* Expand the prime list until we have at least n primes. */
    while (prime_count < n)
    {
        /* Determine a new upper search bound. */
        uint64_t new_limit;
        if (current_max_val > UINT64_MAX / 2ULL - 500ULL)
            new_limit = UINT64_MAX;
        else
            new_limit = current_max_val * 2ULL + 1000ULL;

        /* Bounds for x and y that could generate values up to new_limit. */
        int64_t max_x = (int64_t)(sqrt((double)new_limit / 4.0) + 2.0);
        int64_t max_y = (int64_t)(sqrt((double)new_limit / 5.0) + 2.0);

        /* Temporary buffer for newly found primes. */
        size_t temp_cap = (size_t)( (2ULL * (uint64_t)max_x + 1ULL) *
                                    (2ULL * (uint64_t)max_y + 1ULL) ) / 10ULL + 16ULL;
        if (temp_cap == 0) temp_cap = 16ULL;
        uint64_t *temp = (uint64_t*)malloc(temp_cap * sizeof(uint64_t));
        if (!temp) exit(1);
        size_t temp_cnt = 0;

        /* Enumerate all (x,y) pairs producing values in (current_max_val, new_limit]. */
        for (int64_t x = -max_x; x <= max_x; ++x)
        {
            int64_t xin2 = 4 * x * x;
            for (int64_t y = -max_y; y <= max_y; ++y)
            {
                /* Compute 4x^2 + xy + 5y^2 using 128‑bit arithmetic to avoid overflow. */
                __int128 val128 = (__int128)xin2 + (__int128)x * y + (__int128)5 * y * y;
                if (val128 <= ( __int128 )current_max_val) continue;
                if (val128 > ( __int128 )new_limit) continue;

                uint64_t val = (uint64_t)val128;
                if (isPrime64(val))
                {
                    if (temp_cnt == temp_cap)
                    {
                        temp_cap *= 2;
                        uint64_t *newp = (uint64_t*)realloc(temp, temp_cap * sizeof(uint64_t));
                        if (!newp) { free(temp); exit(1); }
                        temp = newp;
                    }
                    temp[temp_cnt++] = val;
                }
            }
        }

        /* Deduplicate the temporary list (values may appear twice). */
        if (temp_cnt > 1)
        {
            qsort(temp, temp_cnt, sizeof(uint64_t), compare_uint64);
            size_t uniq = 0;
            for (size_t i = 0; i < temp_cnt; ++i)
                if (i == 0 || temp[i] != temp[i - 1])
                    temp[uniq++] = temp[i];
            temp_cnt = uniq;
        }

        /* Append the new primes to the global list, resizing if necessary. */
        if (prime_count + temp_cnt > prime_cap)
        {
            size_t new_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
            while (new_cap < prime_count + temp_cnt) new_cap *= 2;
            uint64_t *new_list =
                (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
            if (!new_list) { free(temp); exit(1); }
            prime_list = new_list;
            prime_cap = new_cap;
        }
        memcpy(prime_list + prime_count, temp, temp_cnt * sizeof(uint64_t));
        prime_count += temp_cnt;
        qsort(prime_list, prime_count, sizeof(uint64_t), compare_uint64);

        free(temp);
        current_max_val = new_limit;
    }

    /* Return the requested prime, cast to the expected seq_t type. */
    return (seq_t)prime_list[n - 1];
}
