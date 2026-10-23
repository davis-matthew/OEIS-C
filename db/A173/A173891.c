/*
 * A173891  Numbers n such that the n-th noncomposite number plus the (n+2)nd
 *          noncomposite number is an even semiprime.
 * https://oeis.org/A173891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A173891 – indices n such that (A008578(n)+A008578(n+2))/2 is prime */

/* External helper functions: */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t)       */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t)   */

/* --- static storage ---------------------------------------------------- */

/* sequence cache: a_cache[0] = a(1), a_cache[1] = a(2), ...           */
static seq_t *a_cache = NULL;
static size_t  a_len   = 0;
static size_t  a_cap   = 0;

/* non‑composite numbers: nc[0] = 1, nc[1] = 2, nc[2] = 3, ...        */
static seq_t *nc        = NULL;
static size_t  nc_len   = 0;
static size_t  nc_cap   = 0;

/* next integer to test as a candidate prime.                           */
static idx_t next_candidate = 2;

/* --- helper functions --------------------------------------------------- */

/* grow an array of seq_t values.                                         */
static void grow_seq_t(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t ncap = (*cap ? *cap << 1 : 16);
        while (ncap < need) ncap <<= 1;
        *arr = (seq_t *)realloc(*arr, ncap * sizeof(seq_t));
        if (!*arr) exit(EXIT_FAILURE);
        *cap = ncap;
    }
}

/* append a value to a_cache.                                            */
static void push_a(seq_t val)
{
    grow_seq_t(&a_cache, &a_cap, a_len + 1);
    a_cache[a_len++] = val;
}

/* append a value to nc.                                                 */
static void push_nc(seq_t val)
{
    grow_seq_t(&nc, &nc_cap, nc_len + 1);
    nc[nc_len++] = val;
}

/* initialise static data (first time a() is called).                    */
static void init_state(void)
{
    if (a_cache) return;        /* already initialised */

    push_nc((seq_t)1);          /* nc[0] = 1   */
}

/* --------------------------------------------------------------------- */
/* a(n) – return the n‑th term of A173891                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1 – return 0 */

    init_state();

    /* generate terms until we have at least n of them                */
    while (a_len < (size_t)n) {

        /* find the next prime integer (candidate for nc)             */
        idx_t cand = next_candidate;
        while (!is_prime(cand)) ++cand;
        next_candidate = cand + 1;

        /* store the new non‑composite number                         */
        push_nc((seq_t)cand);

        /* current position in nc (1‑based) = nc_len                  */
        size_t pos = nc_len;

        /* test the pair only when at least two indices ahead         */
        if (pos >= 3) {
            seq_t first = nc[pos - 3];   /* A008578(pos-2)         */
            seq_t second = nc[pos - 1];  /* A008578(pos)           */
            seq_t sum = first + second;  /* even for pos ≥ 3        */

            if ((sum & (seq_t)1) == 0) { /* sum must be even      */
                seq_t half = sum >> 1;   /* (sum/2)                */
                if (is_prime_big(half))
                    /* add the qualifying index (pos-2).                */
                    push_a((seq_t)(pos - 2));
            }
        }
    }

    return a_cache[(size_t)n - 1];
}
