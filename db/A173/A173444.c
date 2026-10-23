/*
 * A173444  Either (n-th prime-1)^2-+1, but not both, is prime.
 * https://oeis.org/A173444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A173444: the sequence of indices n such that exactly one of
   (p_n-1)^2-1 and (p_n-1)^2+1 is prime, where p_n is the n-th prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t) */
#include "../utils/build_primes.h"  /* void build_primes(idx_t limit,
                                      idx_t **out, size_t *cnt) */

/* static helper to generate primes up to a fixed limit (sufficient for n≤10000) */
static void ensure_prime_table(idx_t **primes, size_t *cnt)
{
    static const idx_t PRIME_LIMIT = 2000000; /* > 10 000-th prime ~104 729 */
    if (*primes == NULL) {
        build_primes(PRIME_LIMIT, primes, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but harmless */

    /* Static state shared across calls */
    static idx_t *prime_list   = NULL;
    static size_t prime_count  = 0;
    static seq_t *table        = NULL;        /* stores found indices */
    static size_t tbl_cap      = 0;
    static size_t tbl_count    = 0;
    static idx_t next_k = 1;                   /* next prime index to test */

    /* Ensure we have a prime table */
    ensure_prime_table(&prime_list, &prime_count);

    /* Initialize result table on first call */
    if (tbl_cap == 0) {
        tbl_cap = 16;
        tbl_count = 0;
        table = (seq_t*)malloc(tbl_cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (tbl_count < n) {
        /* Test the next prime index */
        if (next_k > (idx_t)prime_count) {
            /* This should not happen for n≤10000 with the chosen limit */
            return (seq_t)0;
        }

        idx_t p_k   = prime_list[next_k - 1];          /* p_k */
        idx_t base  = p_k - 1;                         /* p_k-1 */
        idx_t sq    = base * base;                     /* (p_k-1)^2 */

        bool minus_prime = is_prime(sq - 1);
        bool plus_prime  = is_prime(sq + 1);

        /* Exactly one of +1 or -1 must be prime */
        if (minus_prime ^ plus_prime) {
            /* Resize result table if needed */
            if (tbl_count == tbl_cap) {
                tbl_cap *= 2;
                table = (seq_t*)realloc(table, tbl_cap * sizeof(seq_t));
            }
            table[tbl_count++] = (seq_t)next_k;
        }
        next_k++;    /* advance to next prime index */
    }

    return table[n - 1];    /* zero‑based indexing into table */
}
