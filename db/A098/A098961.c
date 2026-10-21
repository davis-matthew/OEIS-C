/*
 * A098961  Sums of two squares and divisible by 13.
 * https://oeis.org/A098961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13 * A001481(n+1)
 * Range:     u8 n <= 11, u16 n <= 1453, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime list for factorisation ---------- */
#define MAX_PRIME 100000
static uint32_t primes_list[MAX_PRIME / 9 + 1]; /* enough to hold ~9592 primes */
static int primes_count = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;

    static uint8_t sieve[MAX_PRIME + 1];
    for (int i = 2; i <= MAX_PRIME; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = 0;
        }
    }

    primes_count = 0;
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (sieve[i]) primes_list[primes_count++] = (uint32_t)i;
    }
    primes_initialized = true;
}

/* ---------- Test for sum of two squares ---------- */
static bool is_sum_of_two_squares(uint64_t n)
{
    if (n == 0) return true;
    uint64_t m = n;
    for (int i = 0; i < primes_count; ++i) {
        uint32_t p = primes_list[i];
        uint64_t pp = (uint64_t)p;
        if (pp * pp > m) break;
        if (m % p == 0) {
            int cnt = 0;
            do {
                m /= p;
                ++cnt;
            } while (m % p == 0);
            if ((p % 4 == 3) && (cnt & 1))
                return false;
        }
    }
    if (m > 1) { /* remaining prime factor > sqrt(original) */
        if (m % 4 == 3) return false;
    }
    return true;
}

/* ---------- Storage for the sequence of sums of two squares ---------- */
static seq_t *sums = NULL;
static size_t sums_len = 0;
static size_t sums_cap = 0;
static uint64_t next_to_test = 0;

static void ensure_sums_cap(size_t needed)
{
    if (sums_cap < needed) {
        size_t new_cap = sums_cap ? sums_cap * 2 : 8;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(sums, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        sums = tmp;
        sums_cap = new_cap;
    }
}

static void generate_upto(idx_t n)
{
    init_primes();
    while (sums_len <= (size_t)n) {
        if (is_sum_of_two_squares(next_to_test)) {
            ensure_sums_cap(sums_len + 1);
            sums[sums_len++] = (seq_t)next_to_test;
        }
        ++next_to_test;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 as safe value */

    if (!sums) {
        sums_cap = 0;
        sums = NULL;
        sums_len = 0;
        next_to_test = 1;          /* start after the 0 we add below */
        ensure_sums_cap(1);
        sums[0] = 0;               /* the first element of A001481 is 0 */
        sums_len = 1;
    }
    generate_upto(n);
    return (seq_t)13 * sums[n];
}
