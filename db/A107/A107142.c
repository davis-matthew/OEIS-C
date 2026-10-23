/*
 * A107142  Primes of the form x^2 + 36y^2.
 * https://oeis.org/A107142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime that can be written as x^2 + 36*y^2 (with
 *            x, y integers and p odd). The sequence starts 37, 61, 157, …
 * Range:     u8 n <= 4, u16 n <= 806, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*   Helper: integer square root for 64‑bit values                     */
/* ------------------------------------------------------------------ */
static uint64_t isqrt_uint64(uint64_t x)
{
    if (x < 2) return x;
    uint64_t hi = x, lo = 1, mid;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;  /* floor(sqrt(x)) */
}

/* ------------------------------------------------------------------ */
/*   Small‑prime generation (used for trial division)                 */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 10000
static uint32_t small_primes[PRIME_LIMIT];
static size_t small_prime_cnt = 0;

static void init_small_primes(void)
{
    if (small_prime_cnt > 0) return;   /* already done */

    bool sieve[PRIME_LIMIT + 1] = {false};
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) sieve[i] = true;
    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) small_primes[small_prime_cnt++] = i;
}

/* ------------------------------------------------------------------ */
/*   Deterministic trial–division primality test for uint64_t          */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)        return false;
    if (n % 2 == 0)   return n == 2;
    if (n % 3 == 0)   return n == 3;

    init_small_primes();

    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint32_t p = small_primes[i];
        uint64_t pp = (uint64_t)p * p;
        if (pp > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*   Find the next prime that can be expressed as x^2 + 36*y^2          */
/* ------------------------------------------------------------------ */
static uint64_t next_candidate = 2;

static uint64_t next_valid_prime(void)
{
    while (true) {
        uint64_t p = next_candidate;
        ++next_candidate;           /* advance for next iteration */

        if (!is_prime_uint64(p))
            continue;

        /* Search y >= 1 such that 36*y^2 <= p and p - 36*y^2 is a square */
        for (uint64_t y = 1;; ++y) {
            uint64_t y2 = y * y;
            uint64_t val = y2 * 36ULL;
            if (val > p) break;     /* y too large */

            uint64_t rem = p - val;
            uint64_t s = isqrt_uint64(rem);
            if (s * s == rem)
                return p;          /* valid representation found */
        }
    }
}

/* ------------------------------------------------------------------ */
/*   Memoized storage of found terms                                  */
/* ------------------------------------------------------------------ */
#define SEQ_CAP 20000
static seq_t  terms[SEQ_CAP];
static size_t terms_size = 0;

/* ------------------------------------------------------------------ */
/*   Public entry point                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                         /* offset is 1; ignore n=0 */
        return 0;

    /* Generate terms until we have at least n of them. */
    while (terms_size < (size_t)n) {
        if (terms_size >= SEQ_CAP)       /* safety: avoid overflow */
            break;

        seq_t val = (seq_t)next_valid_prime();
        terms[terms_size++] = val;
    }

    if ((idx_t)(terms_size) >= n)
        return terms[n - 1];

    /* Should never happen for the given limits; return 0 as a guard. */
    return 0;
}
