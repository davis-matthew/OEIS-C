/*
 * A191248  The number of times that the n-th digit of A019518(n) occurs in
 *          A019518(n).
 * https://oeis.org/A191248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, less
 * Formula:   a(n) = number of times the n-th decimal digit of the
 *            concatenation * of the first n primes occurs in that
 *            concatenation. * (OEIS A191248)
 * Range:     u8 n <= 300, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_count.h"

#define MAX_PRIMES 20000
#define SIEVE_LIMIT 300000

/* global data (static linkage) */
static uint32_t primes[MAX_PRIMES];
static idx_t    prefix_len[MAX_PRIMES + 1];
static seq_t    prefix_counts[MAX_PRIMES + 1][10];
static bool     primes_filled = false;

/* build the list of first MAX_PRIMES primes and cumulative data */
static void generate_primes(void)
{
    if (primes_filled) return;

    bool *is_prime = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= SIEVE_LIMIT; ++p)
        if (is_prime[p])
            for (idx_t q = p * p; q <= SIEVE_LIMIT; q += p)
                is_prime[q] = false;

    prefix_len[0] = 0;
    for (int d = 0; d < 10; ++d) prefix_counts[0][d] = 0;

    idx_t count = 0;
    for (idx_t i = 2; i <= SIEVE_LIMIT && count < MAX_PRIMES; ++i) {
        if (!is_prime[i]) continue;
        primes[count] = (uint32_t)i;

        /* digit counts of the prime */
        uint64_t len = digit_count(i);
        prefix_len[count + 1] = prefix_len[count] + len;

        unsigned char cur[10] = {0};
        uint64_t temp = i;
        while (temp) {
            cur[temp % 10]++;
            temp /= 10;
        }
        for (int d = 0; d < 10; ++d)
            prefix_counts[count + 1][d] = prefix_counts[count][d] + (seq_t)cur[d];

        ++count;
    }
    free(is_prime);
    primes_filled = true;
}

/* digit of a prime (1‑based from left) */
static unsigned char prime_digit(uint32_t p, idx_t pos)
{
    idx_t len = digit_count(p);
    idx_t from_right = len - pos;
    while (from_right--)  p /= 10;
    return (unsigned char)(p % 10);
}

/* required function */
seq_t a(idx_t n)
{
    /* ensure primes are ready */
    if (!primes_filled) generate_primes();
    if (n == 0) return (seq_t)0;   /* safety for n<1, not part of sequence */

    /* binary search for the prime containing the n‑th digit */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (prefix_len[mid] < n) lo = mid + 1;
        else                      hi = mid;
    }
    idx_t prime_idx = lo;                     /* 1‑based prime index */
    idx_t offset   = n - prefix_len[prime_idx - 1];   /* 1‑based offset in that prime */

    unsigned char d = prime_digit(primes[prime_idx - 1], offset);

    return prefix_counts[n][d];
}
