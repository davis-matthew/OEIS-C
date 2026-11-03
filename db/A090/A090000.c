/*
 * A090000  Length of longest contiguous block of 1's in binary expansion of
 *          n-th prime.
 * https://oeis.org/A090000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = length of the longest block of consecutive 1’s in the
 *            binary * representation of the n‑th prime (offset 1)
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 8712 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers (no floating point, no stdio). */
#include <stdint.h>
#include <stddef.h>

/* Longest run of 1's in the binary representation of x. */
static seq_t longest_ones(uint64_t x)
{
    uint64_t cur = 0, max_run = 0;
    while (x) {
        if (x & 1ULL) cur++;
        else {
            if (cur > max_run) max_run = cur;
            cur = 0;
        }
        x >>= 1ULL;
    }
    if (cur > max_run) max_run = cur;
    return (seq_t)max_run;
}

/* A small fixed‑size prime cache – enough for the required n (≈10 000). */
static uint64_t prime_cache[200000];
static size_t prime_cache_cnt = 0;
static uint64_t prime_cache_next = 2;

/* Ensure that prime_cache contains at least the first n primes. */
static void ensure_prime_count(idx_t n)
{
    while (prime_cache_cnt < n) {
        uint64_t cand = prime_cache_next;
        /* Find the next prime by trial division. */
        while (1) {
            int is_prime = 1;
            for (size_t i = 0; i < prime_cache_cnt; ++i) {
                uint64_t p = prime_cache[i];
                if (p * p > cand) break;
                if (cand % p == 0) { is_prime = 0; break; }
            }
            if (is_prime) break;          /* cand is prime */
            ++cand;                        /* try next candidate */
        }
        prime_cache[prime_cache_cnt++] = cand;
        prime_cache_next = cand + 1;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, but harmless */
    ensure_prime_count(n);
    uint64_t p = prime_cache[n - 1];
    return longest_ones(p);
}
