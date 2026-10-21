/*
 * A097386  Numbers n such that (largest digit of n)^(smallest digit of n) + n
 *          is prime.
 * https://oeis.org/A097386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   mulmod64, small_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A097386 – Return the n‑th integer m such that
   (largest decimal digit of m)^(smallest decimal digit of m) + m
   is a prime number.  The function simply scans forward from 1,
   counting terms until the nth is found.  It uses a small‑base
   exponent routine, a deterministic Miller–Rabin test for 64‑bit
   integers, and works for any seq_t that can hold 64‑bit values. */

/* The code is compatible with C11 and C++17.  It does not need
   any external helper functions beyond the standard library. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod64.h"
#include "../utils/small_pow.h"

/*--------- Miller–Rabin for 64‑bit integers --------*/

static inline uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1ULL)
            r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1ULL;
    }
    return r;
}

static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }
    /* Miller–Rabin with bases 2,3,5,7,11,13,17 */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1ULL; ++s; }
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mulmod64(x, x, n);
            if (x == n - 1)
            {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/*--------- main sequence function --------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* undefined for n<1, protect callers */

    idx_t count = 0;
    idx_t candidate = 0;

    while (count < n)
    {
        ++candidate;
        /* obtain largest and smallest decimal digits */
        idx_t t = candidate;
        int largest = 0;
        int smallest = 9;
        if (t == 0)
        {
            smallest = largest = 0;
        }
        else
        {
            while (t > 0)
            {
                int d = (int)(t % 10ULL);
                if (d > largest)  largest = d;
                if (d < smallest) smallest = d;
                t /= 10ULL;
            }
        }

        seq_t add = small_pow(largest, smallest);
        uint64_t test = (uint64_t)candidate + (uint64_t)add;

        if (is_prime_uint64(test))
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }

    /* control never reaches here */
    return (seq_t)0;
}
