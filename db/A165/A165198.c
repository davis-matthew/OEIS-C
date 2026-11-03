/*
 * A165198  Primes from integers by taking the factorial of each digit and
 *          adding them up.
 * https://oeis.org/A165198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the n‑th prime obtainable as the sum of factorials of
 *            the decimal digits of some positive integer. The sequence
 *            records the prime values in the order the corresponding integers
 *            are encountered (duplicates are included)
 * Range:     u8 n <= 14, u16 n <= 5, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* factorial of a single digit (0 … 9) – computed on the fly     */
static seq_t
digit_factorial(int d)
{
    seq_t r = (seq_t)1;
    for (int i = 2; i <= d; ++i)
        r = r * (seq_t)i;
    return r;
}

/* sum of factorials of decimal digits of x */
static seq_t
digit_factorial_sum(idx_t x)
{
    seq_t sum = (seq_t)0;
    if (x == 0)                /* should not happen for our search range */
        return digit_factorial(0);
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        sum += digit_factorial(d);
        x /= 10;
    }
    return sum;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* cache of primes found so far (duplicates allowed) */
static seq_t *prime_cache       = NULL;    /* array of seq_t */
static size_t prime_cache_size = 0;       /* # elements stored */
static size_t prime_cache_cap  = 0;       /* array capacity   */
static idx_t  next_int         = 1;       /* next integer to test */

/* ensure we can write at least 'need' elements into prime_cache */
static void
ensure_capacity(size_t need)
{
    if (prime_cache_cap >= need)
        return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 128;
    if (new_cap < need) new_cap = need;

    /* cast needed for C++ */
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    if (!tmp) abort();          /* out of memory */
    prime_cache      = tmp;
    prime_cache_cap  = new_cap;
}

/* ------------------------------------------------------------ */
/* main sequence function */
seq_t
a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    while (prime_cache_size < (size_t)n) {
        seq_t val = digit_factorial_sum(next_int);
        if (is_prime_big(val)) {
            ensure_capacity(prime_cache_size + 1);
            prime_cache[prime_cache_size++] = val;
        }
        ++next_int;
    }
    return prime_cache[n - 1];
}
