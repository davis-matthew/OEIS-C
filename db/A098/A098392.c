/*
 * A098392  Prime(n)-Log2(Log2(prime(n))), where Log2=A000523.
 * https://oeis.org/A098392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime(n) – Log2(Log2(Prime(n))) (Log2 = floor(log2))
 * Range:     u8 n <= 55, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Twist utility: compute floor(log2(x)) for x>0.  */
static inline uint64_t _floor_log2_uint64(uint64_t x)
{
    /* x must be >0; builtins are safe for 64‑bit values. */
    return 63 - __builtin_clzll(x);
}

/* ---------- Prime generation --------------------------------------- */
static uint64_t primes[20000];      /* enough for n ≤ 10000 (10000th prime < 1.1e6) */
static size_t prime_count = 0;       /* number of primes currently stored */
static uint64_t next_candidate = 2;  /* next integer to test for primeness */

/* Test if a candidate is prime using already stored primes. */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Grow the prime table until at least n primes are available. */
static void ensure_primes(size_t n)
{
    while (prime_count < n) {
        if (is_prime_candidate(next_candidate))
            primes[prime_count++] = next_candidate;
        ++next_candidate;
    }
}

/* ------------- Sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1 – just return 0 */

    /* make sure we have the n‑th prime */
    ensure_primes((size_t)n);

    uint64_t p = primes[n - 1];            /* nth prime (1‑based) */

    /* compute floor(log2(log2(p))) */
    uint64_t inner = _floor_log2_uint64(p);    /* floor(log2(p)) */
    uint64_t outer = _floor_log2_uint64(inner);/* floor(log2(inner)) */

    return (seq_t)p - (seq_t)outer;
}
