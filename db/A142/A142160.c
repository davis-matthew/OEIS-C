/*
 * A142160  Primes congruent to 2 mod 39.
 * https://oeis.org/A142160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 268, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 2 (mod 39) */
/* a(1) = 2, a(2) = 41, a(3) = 197, …   (offset 1) */

#include <stdbool.h>
#include <stddef.h>   /* needed for size_t */

/* Maximum number of terms to cache for fast subsequent requests. */
#define MAX_TERMS 20000

/* Static cache for all primes discovered. */
static seq_t prime_list[MAX_TERMS];
static size_t prime_cnt = 0;

/* Static cache for primes ≡ 2 mod 39. */
static seq_t special_list[MAX_TERMS];
static size_t special_cnt = 0;

/* Next integer to test for primality. */
static seq_t next_candidate = 2;

/* Test whether `n` is prime using the primes already stored. */
static bool is_prime_candidate(seq_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;          /* skip even numbers */

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = prime_list[i];
        /* Stop if p > sqrt(n). */
        if (p > n / p) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Return the nth term of the sequence for 1‑based index n. */
seq_t a(idx_t n)
{
    if (n <= 0) return 0; /* undefined for non‑positive indices */

    /* Generate terms until we have enough. */
    while ((idx_t)special_cnt < n) {
        if (is_prime_candidate(next_candidate)) {
            /* Store prime for future divisor checks. */
            if (prime_cnt < MAX_TERMS)
                prime_list[prime_cnt++] = next_candidate;

            /* Store if it satisfies the congruence. */
            if ((next_candidate % 39) == 2) {
                if (special_cnt < MAX_TERMS)
                    special_list[special_cnt++] = next_candidate;
            }
        }
        ++next_candidate;
    }

    return special_list[n - 1];
}
