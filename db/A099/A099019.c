/*
 * A099019  Odd composite numbers k such that k-2 and k+2 are also composite.
 * https://oeis.org/A099019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   by searching odd composites k such that k-2 and k+2 are also
 *            composite
 * Range:     u8 n <= 17, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Static storage for memoisation */
static seq_t *seq_arr = NULL;   /* array of found terms */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* current capacity of seq_arr  */
static idx_t sieve_limit = 0;   /* last number up to which a sieve was performed */

/* Extend the sequence until at least `min_n` terms are available. */
static void build_until(idx_t min_n)
{
    if (seq_len >= min_n)
        return;

    /* Start with a small limit or double the previous limit. */
    idx_t limit = (sieve_limit == 0) ? 1000 : sieve_limit * 2;

    while (seq_len < min_n) {
        /* Allocate a prime flag array up to limit+2 (needed for k+2). */
        size_t sz = (size_t)(limit + 3);
        unsigned char *is_prime = (unsigned char *)malloc(sz);
        if (!is_prime)
            exit(1);                           /* allocation failure */

        memset(is_prime, 1, sz);
        is_prime[0] = is_prime[1] = 0;

        /* Sieve of Eratosthenes up to limit+2. */
        idx_t up_to = limit + 2;
        for (idx_t p = 2; p * p <= up_to; ++p) {
            if (is_prime[p]) {
                for (idx_t q = p * p; q <= up_to; q += p)
                    is_prime[q] = 0;
            }
        }

        /* Scan odd numbers from the next odd number after previous limit
           up to the new limit, appending those with composite neighbors. */
        idx_t start = (sieve_limit < 3) ? 3 : sieve_limit + 1;
        if ((start & 1) == 0)
            ++start;           /* make it odd */

        for (idx_t k = start; k <= limit; k += 2) {
            if (!is_prime[(size_t)k]) {                 /* k is composite */
                if (!is_prime[(size_t)(k - 2)] && !is_prime[(size_t)(k + 2)]) {
                    /* Grow the term array if needed. */
                    if (seq_len == seq_cap) {
                        size_t new_cap = seq_cap ? seq_cap * 2 : 8;
                        seq_t *tmp = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
                        if (!tmp)
                            exit(1);      /* allocation failure */
                        seq_arr = tmp;
                        seq_cap = new_cap;
                    }
                    seq_arr[seq_len++] = (seq_t)k;
                }
            }
        }

        free(is_prime);
        sieve_limit = limit;     /* remember the new sieve limit */

        /* If still not enough terms, double the limit for the next pass. */
        if (seq_len < min_n)
            limit <<= 1;
    }
}

/* Public interface: a(n) returns the nth element of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n < 1 */

    build_until(n);
    return seq_arr[n - 1];
}
