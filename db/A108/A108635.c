/*
 * A108635  Semiprimes with property that digits are in nonincreasing order.
 * https://oeis.org/A108635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   decreasing_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined as the semiprimes whose decimal digits are in
   non‑increasing order (i.e. each digit is not larger than the preceding
   one).  The algorithm is a simple generator: we test every integer
   starting at 4, keep the prime table for trial division and store terms
   as they are found.  It runs in O(n·p) time where p is the
   enumeration of primes up to √x; this is more than fast enough for
   n≈10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/decreasing_digits.h"

/* integer square root for 64‑bit values --------------------------------- */
static inline uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t lo = 0, hi = n;
    while (lo < hi) {
        uint64_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* prime table that is grown as needed ----------------------------------- */
static uint64_t *prime_tbl  = NULL;
static size_t   prime_cnt   = 0;
static size_t   prime_cap   = 0;
static uint64_t next_prime_candidate = 2;

static void push_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 128;
        uint64_t *tmp = (uint64_t *)realloc(prime_tbl, new_cap * sizeof(uint64_t));
        prime_tbl = tmp;
        prime_cap = new_cap;
    }
    prime_tbl[prime_cnt++] = p;
}

static void ensure_primes_upto(uint64_t limit)
{
    while (next_prime_candidate <= limit) {
        bool is_p = true;
        uint64_t r = isqrt_uint64(next_prime_candidate);
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_tbl[i];
            if (p > r)
                break;
            if (next_prime_candidate % p == 0) {
                is_p = false;
                break;
            }
        }
        if (is_p)
            push_prime(next_prime_candidate);
        next_prime_candidate++;
    }
}

/* check whether a number is a semiprime (product of exactly two primes
   with multiplicity 2).  The function uses trial division by the primes
   that have been found so far. ------------------------------------------------ */
static bool is_semiprime(uint64_t x)
{
    int factors = 0;
    uint64_t m = x;
    uint64_t limit = isqrt_uint64(m);
    ensure_primes_upto(limit);
    for (size_t i = 0; i < prime_cnt && prime_tbl[i] <= limit; ++i) {
        uint64_t p = prime_tbl[i];
        while (m % p == 0) {
            ++factors;
            m /= p;
            if (factors > 2)
                return false;
        }
        if (m == 1)
            break;
        limit = isqrt_uint64(m);
    }
    if (m > 1)
        ++factors;   /* m itself is prime */
    return factors == 2;
}

/* dynamic array for the sequence values ---------------------------------- */
static seq_t *seq_arr = NULL;
static size_t seq_sz  = 0;
static size_t seq_cap = 0;
static uint64_t next_num = 4;  /* smallest possible semiprime */

static void push_seq(seq_t val)
{
    if (seq_sz == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_arr = tmp;
        seq_cap = new_cap;
    }
    seq_arr[seq_sz++] = val;
}

/* required function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1; behaviour for n<1 is irrelevant */

    /* generate terms until reaching the requested index */
    while (seq_sz < (size_t)n) {
        uint64_t cand = next_num++;
        if (!decreasing_digits(cand))
            continue;
        if (!is_semiprime(cand))
            continue;
        push_seq((seq_t)cand);
    }
    return seq_arr[n - 1];
}
