/*
 * A109277  Slowest increasing sequence in which a(n) is a prime closest to
 *          the sum of all previous terms.
 * https://oeis.org/A109277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the prime closest to the sum of all previous terms; if
 *            two primes are equally close, choose the smaller one
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint64_t */
#include <stdbool.h>  /* bool */

#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/* Global state holding the sequence. */
static seq_t  *seq = NULL;      /* array of computed terms */
static size_t  cap  = 0;        /* allocated capacity of seq   */
static size_t  size = 0;        /* number of terms stored      */
static seq_t   sum  = 0;        /* sum of all computed terms   */

/* Multiply a and b modulo mod using a method that avoids overflow
   for both ordinary integer and arbitrary‑precision types. */
static seq_t
mod_mul(const seq_t a, const seq_t b, const seq_t mod)
{
    seq_t res   = 0;
    seq_t aa    = a % mod;
    seq_t bb    = b;
    while (bb != 0) {
        if (bb & 1)
            res = (res + aa) % mod;
        aa = (aa << 1) % mod;
        bb >>= 1;
    }
    return res;
}

/* Compute (base ^ exp) mod mod by binary exponentiation. */
static seq_t
mod_pow(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp != 0) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin test for 64‑bit numbers and a probabilistic
   version for larger sizes.  Works for all seq_t that support the usual
   arithmetic operations. */
static bool
is_prime(const seq_t n)
{
    if (n < 2)
        return false;

    static const uint32_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,
        31,37,41,43,47,53,59,61,67,
        71,73,79,83,89,97
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        seq_t sp = (seq_t)small_primes[i];
        if (n == sp)
            return true;
        if (n % sp == 0)
            return false;
    }

    /* Write n-1 as d * 2^s with d odd. */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        seq_t a = (seq_t)bases[i];
        if (a > n - 2)
            continue;   /* base larger than n-2 : skip */
        seq_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool comp = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                comp = true;
                break;
            }
        }
        if (!comp)
            return false;
    }
    return true;
}

/* Find the prime closest to target; ties are resolved in favour of the
   smaller prime.  Works for both ordinary and arbitrary‑precision
   seq_t values. */
static seq_t
find_closest_prime(const seq_t target)
{
    if (target < 2)
        return 2;

    uint64_t delta = 0;
    for (;;) {
        if (delta == 0) {
            if (is_prime(target))
                return target;
        } else {
            /* lower candidate */
            seq_t low = (target > delta) ? (target - delta) : 0;
            if (low >= 2 && is_prime(low))
                return low;

            /* higher candidate */
            seq_t high = target + delta;
            if (high > target && is_prime(high))
                return high;
        }
        ++delta;
    }
}

/* Ensure that the static array is large enough to hold at least
   'needed' elements. */
static void
ensure_capacity(size_t needed)
{
    if (!seq) {
        cap = (needed < 16 ? 16 : needed);
        seq = seq_malloc((idx_t)cap);
    } else if (cap < needed) {
        cap = needed * 2;
        seq = seq_realloc(seq, (idx_t)cap);
    }
}

/* OEIS sequence A109277: a(n) as described above. */
seq_t
a(idx_t n)
{
    if (n == 0)          /* offset 1 – outside valid range */
        return 0;

    /* Initialise the first term */
    if (size == 0) {
        ensure_capacity(1);
        seq[0] = 2;
        sum    = 2;
        size   = 1;
    }

    /* Compute further terms up to the requested index */
    while (size < (size_t)n) {
        seq_t prime = find_closest_prime(sum);
        ensure_capacity(size + 1);
        seq[size] = prime;
        sum += prime;
        ++size;
    }

    return seq[n - 1];
}
