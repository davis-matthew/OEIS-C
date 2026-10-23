/*
 * A156074  Triangle read by rows: T(n, k) = 3 + prime(n+1) - prime(k+1) -
 *          prime(n-k+1).
 * https://oeis.org/A156074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3 + prime(r+1) - prime(k+1) - prime(r-k+1) where (r,k)
 *            are the row and column of the n‑th entry in the triangle T(r,k)
 *            = 3 + prime(r+1) - prime(k+1) - prime(r-k+1)
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Keep the first few primes in a static array.  For n ≤ 10 000
   the row index r is at most 141, so we only need the first 150
   primes.  A simple trial‑division generator is fast enough. */
#define MAX_PRIME_COUNT 200
static uint32_t primes_buf[MAX_PRIME_COUNT];
static size_t primes_cnt = 0;

/* Generate the first 'need' primes and store them in primes_buf. */
static void generate_primes(size_t need)
{
    size_t i = 0;
    uint32_t candidate = 2;
    while (i < need) {
        bool is_prime = true;
        for (size_t j = 0; j < i; ++j) {
            uint32_t p = primes_buf[j];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes_buf[i++] = candidate;
        }
        ++candidate;
    }
    primes_cnt = need;
}

/* Ensure that at least 'need' primes are available. */
static void ensure_primes(size_t need)
{
    if (primes_cnt >= need) return;
    generate_primes(need);
}

/* a(n): compute the n-th term of A156074 with arbitrary‑precision
   or fixed‑width integer type seq_t. */
seq_t a(idx_t n)
{
    /* Locate the row (r) and column (k) for the flattened index n. */
    idx_t r = 0;
    idx_t rem = n;
    while (rem >= (r + 1)) {
        rem -= (r + 1);
        ++r;
    }
    idx_t k = rem;                       /* 0‑based column index */

    /* Make sure we have enough primes for indices up to r+1. */
    ensure_primes((size_t)(r + 1));

    /* Compute the triangle entry. */
    seq_t val = (seq_t)3
                + (seq_t)primes_buf[(size_t)r]          /* prime(r+1)   */
                - (seq_t)primes_buf[(size_t)k]          /* prime(k+1)   */
                - (seq_t)primes_buf[(size_t)(r - k)];   /* prime(r-k+1) */

    return val;
}
