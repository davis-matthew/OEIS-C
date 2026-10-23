/*
 * A180655  Semiprimes (products of two distinct primes) of the form, next
 *          perfect square minus semiprime equals perfect square.
 * https://oeis.org/A180655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A180655:
 *   a(n) = semiprime p*q (p≠q) such that the next perfect square after
 *   n is a perfect square minus n.
 *   In other words, if m = p*q then (⌊√m⌋+1)² – m is a perfect square.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Limit for prime sieve – large enough for the first 10 000 terms
 * (the largest found < 2·10⁸).  Adjust if needed. */
#define PRIME_LIMIT 2000000

static bool composite[PRIME_LIMIT + 1];
static uint32_t primes[PRIME_LIMIT / 10];
static size_t prime_cnt = 0;

/* Initialise the sieve and prime list only once. */
static void init_primes(void)
{
    if (prime_cnt != 0) return;              /* already initialised */

    primes[prime_cnt++] = 2;
    for (uint32_t i = 3; i <= PRIME_LIMIT; i += 2) {
        if (!composite[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += 2 * i)
                    composite[j] = true;
            }
        }
    }
}

/* Integer square root for seq_t (works for 8/16/32/64/128-bit and arbitrary‑precision). */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t sq  = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  low = mid + 1;
        else if (mid == 0) break;
        else          high = mid - 1;
    }
    return high;
}

/* Quick test whether n is a product of two distinct primes (semiprime). */
static bool is_semiprime_distinct(uint64_t n)
{
    uint64_t temp = n;
    int count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;
        if (temp % p == 0) {
            ++count;
            temp /= p;
            if (temp % p == 0) return false;   /* repeated factor */
        }
    }
    if (temp > 1) ++count;
    return count == 2;
}

/* Dynamic array of cached terms. */
static seq_t *terms = NULL;
static size_t terms_size = 0;   /* number of terms stored */
static size_t terms_cap  = 0;   /* capacity of the array */
static uint64_t current_candidate = 15;      /* start from first known term */

/* Resize the cache when required. */
static void grow_terms(size_t needed)
{
    size_t new_cap = terms_cap ? terms_cap : 128;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    terms = tmp;
    terms_cap = new_cap;
}

/* Make sure we have at least n terms cached. */
static void ensure_terms(idx_t n)
{
    init_primes();
    while (terms_size < (size_t)n) {
        /* Find the next qualifying semiprime. */
        while (true) {
            if (is_semiprime_distinct(current_candidate)) {
                seq_t m = current_candidate;
                seq_t sqrt_floor = isqrt_seq(m);
                seq_t next_sq   = (sqrt_floor + 1) * (sqrt_floor + 1);
                seq_t diff      = next_sq - m;
                seq_t sqrt_diff = isqrt_seq(diff);
                if (sqrt_diff > 0 && sqrt_diff * sqrt_diff == diff) {
                    if (terms_size >= terms_cap) grow_terms(terms_cap + 1);
                    terms[terms_size++] = m;
                    /* Move to next candidate to avoid repeats. */
                    current_candidate++;
                    break;
                }
            }
            current_candidate++;
        }
    }
}

/* The exported sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
