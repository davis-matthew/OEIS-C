/*
 * A107073  Numbers k such that the string 35k is prime.
 * https://oeis.org/A107073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest k such that the decimal string “35k” represents
 *            a prime number. The algorithm searches for successive k by
 *            testing 35*10^d + k for primality, where d is the number of
 *            decimal digits of k. It memoizes found values so that calls with
 *            arbitrary n are answered in O(1) after the initia...
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* deterministic Miller–Rabin for 64‑bit integers */
static uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static uint64_t powmod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e)
    {
        if (e & 1) res = mulmod(res, a, mod);
        a = mulmod(a, a, mod);
        e >>= 1;
    }
    return res;
}

static bool isPrimeUint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Bases that make Miller–Rabin deterministic for 64‑bit numbers */
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mulmod(x, x, n);
            if (x == n - 1)
            {
                cont = true;
                break;
            }
        }
        if (!cont) return false;
    }
    return true;
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq   = NULL;
    static size_t   sz  = 0;
    static size_t   cap = 0;
    static idx_t    last_k      = 0;
    static uint64_t pow10      = 10;   /* 10^current_digit_count */
    static uint64_t next_limit = 10;   /* next value at which digit count increases */

    if (n == 0) return 0; /* defined only for n >= 1, but guard anyway */

    /* grow the table until at least n elements are available */
    while (sz < (size_t)n)
    {
        /* next candidate k */
        ++last_k;

        /* increase the digit count when we reach the next limit */
        if (last_k == next_limit)
        {
            pow10     *= 10;
            next_limit*= 10;
        }

        uint64_t candidate = 35 * pow10 + last_k;
        if (isPrimeUint64(candidate))
        {
            if (sz == cap)
            {
                cap = cap ? cap * 2 : 4;
                seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
                if (!seq) exit(1); /* allocation failure */
            }
            seq[sz++] = (seq_t)last_k;
        }
    }

    return seq[n - 1];
}
