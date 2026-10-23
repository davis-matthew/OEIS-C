/*
 * A023884  Average of divisors except itself is an integer.
 * https://oeis.org/A023884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth n (n ≥ 2) such that average of proper divisors is
 *            integer i.e. (σ(n) – n)/(τ(n) – 1) is an integer
 * Range:     u8 n <= 88, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: integer square‑root approximation to bound prime tests. */
static idx_t _floor_isqrt_u64(idx_t n);
/* Forward declarations for prime list management */
static void ensure_primes_upto(uint64_t limit);
static void append_prime(uint64_t p);

/* Cache for sequence values */
static seq_t *seq_cache = NULL;      /* dynamic array of found terms */
static size_t seq_len  = 0;           /* number of elements stored */
static size_t seq_cap  = 0;           /* allocated capacity */

/* Cache for prime numbers used in factorisation */
static uint64_t *prime_list = NULL;
static size_t prime_len    = 0;
static size_t prime_cap    = 0;

/* -------------------------------------------------------------------------- */
static idx_t _floor_isqrt_u64(idx_t n)
{
    /* binary search for floor(sqrt(n)) */
    idx_t lo = 0, hi = (idx_t) (n >> 1) + 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (mid > 0 && mid <= n / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

/* -------------------------------------------------------------------------- */
static void ensure_primes_upto(uint64_t limit)
{
    if (limit <= (uint64_t)prime_len) return;

    if (prime_cap == 0) {
        prime_cap = 64;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    } else if (prime_cap < limit + 1) {
        while (prime_cap < limit + 1) prime_cap <<= 1;
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }

    for (uint64_t cand = (uint64_t)(prime_len > 0 ? prime_list[prime_len-1] : 1) + 1;
         cand <= limit; ++cand) {
        bool is_p = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_p = false; break; }
        }
        if (is_p) {
            if (prime_len == prime_cap) {
                /* safety, although we resized earlier */
                prime_cap <<= 1;
                prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = cand;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Test whether n satisfies (σ(n)-n)/(τ(n)-1) is integer. */
static bool good_number(uint64_t n)
{
    if (n < 2) return false;        /* skip 1 */
    ensure_primes_upto(_floor_isqrt_u64(n));

    uint64_t m = n;
    seq_t sigma = 1;   /* product of sigma(p^e) parts */
    int tau = 1;       /* product of tau(p^e) parts */

    for (size_t i = 0; i < prime_len && prime_list[i] * prime_list[i] <= m; ++i) {
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            int exp = 0;
            seq_t term = 1;
            seq_t pow = 1;
            while (m % p == 0) {
                m /= p;
                exp++;
                pow *= (seq_t)p;
                term += pow;
            }
            sigma *= term;
            tau *= (exp + 1);
        }
    }
    if (m > 1) {                /* remaining prime factor */
        sigma *= (seq_t)(1 + m);
        tau *= 2;
    }

    if (tau <= 1) return false;   /* should not happen except n=1 */
    seq_t sum_proper   = sigma - (seq_t)n;
    seq_t count_proper = (seq_t)(tau - 1);
    return (sum_proper % count_proper) == 0;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
/* Return the nth term of A023884 (offset 1). */
{
    if (n == 0) return 0;       /* undefined, return 0 */

    /* Lazy generation of sequence terms. */
    static idx_t next_candidate = 2;   /* start scanning from 2 */
    if (seq_cache == NULL) {
        seq_cap = 64;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    }

    while (seq_len < (size_t)n) {
        if (good_number((uint64_t)next_candidate)) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return seq_cache[n - 1];
}
