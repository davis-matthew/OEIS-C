/*
 * A079066  "Memory" of prime(n): the number of distinct (previous) primes
 *          contained as substrings in prime(n).
 * https://oeis.org/A079066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A039997(prime(n)) - 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------------------------------------------ */
/*   Prime sieve – incremental storage of the first primes.                 */
/* ------------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t     prime_cnt  = 0;     /* number of primes stored */
static size_t     prime_cap  = 0;     /* allocated capacity */

/* generate primes until we have at least n primes */
static void ensure_prime_count(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;                       /* already enough */

    if (prime_cnt == 0) {              /* first prime is 2 */
        prime_list = (uint64_t *)malloc(64 * sizeof(uint64_t));
        if (!prime_list) exit(1);
        prime_cap = 64;
        prime_list[prime_cnt++] = 2;
    }

    uint64_t candidate = prime_list[prime_cnt - 1] + 1;  /* start after last prime */
    while (prime_cnt < (size_t)n) {
        bool isprime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { isprime = false; break; }
        }
        if (isprime) {
            if (prime_cnt == prime_cap) {        /* grow array */
                size_t new_cap = prime_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                     new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------------ */
/*   Binary search for a value in prime_list[0..hi-1].                       */
/* ------------------------------------------------------------------------ */
static int binary_search_uint64(uint64_t val, size_t lo, size_t hi)
{
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        uint64_t midval = prime_list[mid];
        if (midval < val)
            lo = mid + 1;
        else if (midval > val)
            hi = mid;
        else
            return (int)mid;
    }
    return -1;
}

/* ------------------------------------------------------------------------ */
/*   Main sequence function                                                 */
/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* get the n-th prime */
    ensure_prime_count(n);                 /* guarantees prime_list[0..n-1] exist */
    uint64_t p = prime_list[(size_t)n - 1];   /* the n‑th prime */

    /* convert p to decimal string – buffer big enough for 64‑bit values */
    char buf[25];
    int len = 0;
    uint64_t t = p;
    if (t == 0) buf[len++] = '0';
    while (t != 0) {
        buf[len++] = (char)('0' + (t % 10));
        t /= 10;
    }
    /* reverse to obtain the correct order */
    for (int i = 0; i < len / 2; ++i) {
        char c = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = c;
    }

    /* array marking which of the first n primes have already been seen */
    bool *seen = (bool *)calloc(prime_cnt, sizeof(bool));
    if (!seen) exit(1);
    size_t count = 0;

    for (int start = 0; start < len; ++start) {
        uint64_t val = 0;
        for (int end = start; end < len; ++end) {
            val = val * 10 + (buf[end] - '0');
            if (val < 2) continue;           /* 0 or 1 are not prime */
            if (val > p) break;              /* longer substrings will be larger */
            int idx = binary_search_uint64(val, 0, prime_cnt);
            if (idx >= 0 && (size_t)idx < n && !seen[(size_t)idx]) {
                seen[(size_t)idx] = true;
                ++count;
            }
        }
    }

    free(seen);
    /* subtract one to exclude the prime itself (formula uses A039997 - 1) */
    return (seq_t)(count - 1);
}
