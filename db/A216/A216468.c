/*
 * A216468  Primes p with property that there exists a number d>0 such that
 *          numbers p-k*d, k=1...6, are six primes.
 * https://oeis.org/A216468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth prime p such that there exists a positive integer d with
   p – k·d (k = 1…6) all prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper macros for bit‑set primality test */
#define BITSET_SET(arr, n)      ((arr)[(n)>>3] &= ~(1u<<((n)&7)))
#define BITSET_GET(arr, n)      (((arr)[(n)>>3] >> ((n)&7)) & 1u)

/* Global storage for the sequence */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;   /* number of terms stored */
static size_t seq_cap  = 0;   /* allocated capacity of seq */

/* Build the sequence up to the given prime limit (inclusive). */
static void build_seq_upto(idx_t limit)
{
    /* Free any previous sequence data */
    if (seq) { free(seq); seq = NULL; }
    seq_len = seq_cap = 0;

    /* Allocate bitset: one bit per odd integer up to limit
       (use all numbers for simplicity). */
    size_t bits = (size_t)limit + 1;
    size_t sz = (bits >> 3) + 1;
    uint8_t *is_prime = (uint8_t *)malloc(sz);
    if (!is_prime) return;                /* memory error – abort quietly */

    /* Initially assume all numbers are prime */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = 0xFFU;
    /* Mark 0 and 1 as not prime */
    BITSET_SET(is_prime, 0);
    BITSET_SET(is_prime, 1);

    /* Eratosthenes sieve */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (BITSET_GET(is_prime, i))
            for (idx_t j = i * i; j <= limit; j += i)
                BITSET_SET(is_prime, j);
    }

    /* Collect qualifying primes */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!BITSET_GET(is_prime, p)) continue;   /* not prime */

        idx_t max_d = (p > 2) ? (p - 2) / 6 : 0;
        bool found = false;
        for (idx_t d = 1; d <= max_d; ++d) {
            bool ok = true;
            for (int k = 1; k <= 6 && ok; ++k) {
                idx_t val = p - k * d;
                if (!BITSET_GET(is_prime, val)) ok = false;
            }
            if (ok) { found = true; break; }
        }
        if (found) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)p;
        }
    }

    free(is_prime);
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) is undefined – return 0 */

    /* If sequence already long enough, return the term. */
    if (seq_len >= n) return seq[n - 1];

    /* Start with a modest bound and double until we have enough terms. */
    idx_t limit = 400000;                 /* good initial bound for n ≤ 10000 */
    while (seq_len < n) {
        build_seq_upto(limit);
        if (seq_len >= n) break;
        /* If we didn't get enough terms, increase the bound dramatically. */
        limit *= 2;                      /* protective safety */
    }

    return seq[n - 1];
}
