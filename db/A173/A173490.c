/*
 * A173490  Even abundant numbers (even numbers n whose sum of divisors
 *          exceeds 2n).
 * https://oeis.org/A173490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * A039725(n) // even abundant numbers
 * Range:     u8 n <= 58, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square root using binary search */
static idx_t int_sqrt(idx_t n)
{
    idx_t low = 0, high = n, mid;
    while (low <= high) {
        mid = low + (high - low) / 2;
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* ---------- Prime generation ---------- */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;
static uint64_t next_prime_candidate = 2;

/* Extend the prime list so that it contains all primes ≤ limit. */
static void extend_primes_upto(uint64_t limit)
{
    if (primes_cnt && primes[primes_cnt - 1] >= limit) return;
    while (next_prime_candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p > next_prime_candidate / p) break;   /* p*p ≤ candidate? */
            if (next_prime_candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap << 1 : 16;
                primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = next_prime_candidate;
        }
        ++next_prime_candidate;
    }
}

/* ---------- Abundant test for an even number ---------- */
static bool is_abundant_even(uint64_t n)
{
    if (n < 2) return false;
    uint64_t temp = n;
    uint64_t sigma = 1;                /* multiplicative identity for σ */
    uint64_t limit = int_sqrt(temp);
    extend_primes_upto(limit);
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > temp / p) break;        /* stop when p*p > temp */
        if (temp % p == 0) {
            /* factor p^e */
            uint64_t term_sum = 1;      /* 1 + p + p² + … + pᵉ */
            uint64_t p_pow = 1;
            while (temp % p == 0) {
                temp /= p;
                p_pow *= p;
                term_sum += p_pow;
            }
            sigma *= term_sum;
            /* early exit: once σ > 2n, it can never decrease */
            if (sigma > 2ULL * n) return true;
        }
    }
    if (temp > 1) {
        /* remaining prime factor */
        sigma *= (1ULL + temp);
    }
    return sigma > 2ULL * n;
}

/* ---------- Cache for even abundant numbers ---------- */
static seq_t *even_abundants = NULL;
static idx_t even_abund_cnt = 0;
static idx_t even_abund_cap = 0;
static uint64_t next_candidate = 2;   /* next even number to test */

/* Append a new even abundant number to the cache. */
static void append_even_abundant(seq_t val)
{
    if (even_abund_cnt == even_abund_cap) {
        idx_t new_cap = even_abund_cap ? even_abund_cap << 1 : 16;
        even_abundants = (seq_t *)realloc(even_abundants, new_cap * sizeof(seq_t));
        even_abund_cap = new_cap;
    }
    even_abundants[even_abund_cnt++] = val;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence starts at n = 1 */

    while (even_abund_cnt < n) {
        /* ensure we have all primes up to √next_candidate */
        uint64_t limit = int_sqrt((idx_t)next_candidate);
        extend_primes_upto(limit);

        if (is_abundant_even(next_candidate))
            append_even_abundant((seq_t)next_candidate);

        next_candidate += 2;   /* move to the next even number */
    }
    return even_abundants[n - 1];
}
