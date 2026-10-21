/*
 * A079447  Primes p such that there is an integer k satisfying p =
 *          floor(k*H(k)) where H(k) denotes the k-th harmonic number (i.e.,
 *          H(k) = 1 + 1/2 + 1/3 + ... + 1/k).
 * https://oeis.org/A079447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p = floor(k * H(k)), where
   H(k) = 1 + 1/2 + … + 1/k. */

/* -----------------------------------------------------------------------
 *   64‑bit Miller–Rabin implementation (deterministic for all 64‑bit
 *   integers).  The implementation is plain C and therefore works
 *   for the C build.  In the C++ build the same code is used – it
 *   also compiles and works with arbitrary‑precision integer types
 *   because all intermediate results are 64‑bit.
 * ----------------------------------------------------------------------- */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Multiplication modulo mod, using the 128‑bit intermediate. */
static inline uint64_t mul_mod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((unsigned __int128)a * b % mod);
}

/* Power modulo: a^e mod mod.  e is 64‑bit, mod is 64‑bit. */
static inline uint64_t pow_mod_u64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e)
    {
        if (e & 1) r = mul_mod_u64(r, a, mod);
        a = mul_mod_u64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for all unsigned 64‑bit integers. */
static bool isPrime_u64(uint64_t n)
{
    if (n < 2) return false;

    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
        23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    /* Write n-1 as d * 2^s with d odd. */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Bases that give deterministic test for 64‑bit integers. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        uint64_t x = pow_mod_u64(a, d, n);
        if (x == 1 || x == n-1) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod_u64(x, x, n);
            if (x == n-1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* -----------------------------------------------------------------------
 *   Sequence generator.  The function is memoised and works in any
 *   order of calls.  The array of primes in the sequence is stored
 *   in dynamically allocated memory.  This works for any build
 *   where seq_t is an unsigned integer type (or an arbitrary‑precision
 *   integer class that accepts a 64‑bit integer literal).
 * ----------------------------------------------------------------------- */

static seq_t       *seq   = NULL;   /* array of found primes */
static size_t       seq_len = 0;     /* number of primes stored */
static size_t       seq_cap = 0;     /* allocated capacity */

static idx_t       last_k   = 0;     /* largest k processed so far */
static long double harmonic = 0.0L;  /* H_k = 1 + … + 1/k */

/* -----------------------------------------------------------------------
 *   a(n) returns the n‑th term, for n >= 1.  For n == 0 a value that
 *   is not used (here 0) is returned; the OEIS offset is 1.
 * ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 – behaviour below irrelevant */

    while (seq_len < n)
    {
        ++last_k;
        harmonic += 1.0L / last_k;          /* H_last_k = H_{k-1} + 1/k */

        long double prod = (long double)last_k * harmonic;
        uint64_t val64 = (uint64_t)floorl(prod);   /* floor(k * H_k) */

        if (isPrime_u64(val64))
        {
            if (seq_len >= seq_cap)
            {
                size_t new_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!new_ptr) abort();          /* allocation failed – terminate */
                seq = new_ptr;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)val64;
        }
    }

    return seq[n-1];
}
