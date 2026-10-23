/*
 * A109600  Numbers n such that number of 1's in binary representation of n
 *          equals the number of digits required to write the prime factors of
 *          n.
 * https://oeis.org/A109600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digits_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A109600:  numbers n whose binary popcount equals the total
    decimal digit count of the prime factors of n (multiplied
    by their multiplicity).  This routine generates the
    sequence on demand and memoises the found terms. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/digits_of.h"

/* ------------------------------------------------------------------ */
/*---  Helper functions ------------------------------------------------- */
static inline uint32_t popcount64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    uint32_t c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
#endif
}

/* ------------------------------------------------------------------ */
/*---  Prime list maintenance ------------------------------------------ */
static uint32_t *primes = NULL;          /* stored primes            */
static size_t  prime_cnt = 0;            /* number of stored primes  */
static size_t  prime_cap = 0;            /* allocated capacity       */
static uint32_t prime_candidate = 2;     /* next number to test     */

static void ensure_primes_upto(uint32_t limit)
{
    while (prime_candidate <= limit)
    {
        bool is_prime = true;
        uint32_t lim = (uint32_t) sqrt((double) prime_candidate);
        for (size_t i = 0; i < prime_cnt && primes[i] <= lim; ++i)
        {
            if (prime_candidate % primes[i] == 0) { is_prime = false; break; }
        }
        if (is_prime)
        {
            if (prime_cap == prime_cnt)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint32_t *tmp = (uint32_t *) realloc(primes, prime_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[prime_cnt++] = prime_candidate;
        }
        ++prime_candidate;
    }
}

/* ------------------------------------------------------------------ */
/*---  Check whether a number satisfies the condition ---------------- */
static bool good_value(uint64_t val)
{
    int target = popcount64(val);
    uint64_t tmp = val;
    int digitSum = 0;

    ensure_primes_upto((uint32_t) sqrt((double) tmp));

    for (size_t i = 0; i < prime_cnt && (uint64_t) primes[i] * (uint64_t) primes[i] <= tmp; ++i)
    {
        uint32_t p = primes[i];
        int cnt = 0;
        while (tmp % p == 0)
        {
            tmp /= p;
            ++cnt;
        }
        if (cnt) digitSum += cnt * digits_of(p);
    }
    if (tmp > 1)                     /* remaining prime factor */
        digitSum += digits_of(tmp);

    return target == digitSum;
}

/* ------------------------------------------------------------------ */
/*---  Memoised sequence storage ------------------------------------- */
static seq_t *seq_arr = NULL;    /* values */
static idx_t  seq_size = 0;      /* number of values stored */
static idx_t  seq_cap  = 0;      /* allocated capacity */
static uint64_t last_checked = 1;/* last n processed */

/* ------------------------------------------------------------------ */
/*---  Core routine --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n>=1 */

    while (seq_size < n)
    {
        ++last_checked;
        if (good_value(last_checked))
        {
            if (seq_cap == seq_size)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *) realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                seq_arr = tmp;
            }
            seq_arr[seq_size++] = (seq_t) last_checked;
        }
    }
    return seq_arr[n-1];
}
