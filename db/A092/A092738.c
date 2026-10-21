/*
 * A092738  Primes of the form prime(x)+prime(x+1)+1.
 * https://oeis.org/A092738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 902, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092738: primes of the form prime(k)+prime(k+1)+1
   Offset: 1  (a(1)=13)

   The function generates successive primes p = prime(k)+prime(k+1)+1
   by first building a prime list up to a fixed limit (10 000 000) using
   a simple sieve, then iterating over that list.  When the nth element
   is required, it continues the scan until it has found at least n
   such primes.  Only the small set of primes needed for the first
   few thousand terms is required, so this approach is fast enough
   for n ≤ 10 000 and works for any idx_t and seq_t.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* forward declaration – the build is done inside this file */
static void build_prime_table(void);
static inline bool is_prime_u64(uint64_t x);

/* static state */
static bool primes_built = false;
static uint64_t *prime_vals = NULL;     /* array of primes (idx_t) */
static size_t prime_cnt = 0;            /* number of primes */

static size_t current_k = 0;            /* index of next k to test */

static seq_t *term_vals = NULL;         /* results a(1), a(2), … */
static size_t term_cap = 0;
static size_t term_len = 0;

/* ------------------------------------------------------------------ */
/* Build prime list up to a hard‑coded limit (10 000 000). */
static void
build_prime_table(void)
{
    const uint64_t LIMIT = 10000000ULL;

    /* Allocate marker array (1 = composite, 0 = prime candidate) */
    unsigned char *is_comp = (unsigned char *)calloc(LIMIT + 1, 1);
    if (!is_comp) {
        /* out of memory – abort */
        abort();
    }

    /* Sieve of Eratosthenes */
    uint64_t i, j;
    for (i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (j = i * i; j <= LIMIT; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Count primes first to allocate array */
    size_t cnt = 0;
    for (i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) ++cnt;
    }

    prime_vals = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_vals) {
        free(is_comp);
        abort();
    }

    size_t idx = 0;
    for (i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            prime_vals[idx++] = i;
        }
    }
    prime_cnt = cnt;

    free(is_comp);
    primes_built = true;
}

/* ------------------------------------------------------------------ */
/* Very small primality test using the prebuilt prime list. */
static inline bool
is_prime_u64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_vals[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Append a new term to the cached list. */
static void
append_term(seq_t val)
{
    if (term_len == term_cap) {
        size_t newcap = term_cap ? term_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(term_vals, newcap * sizeof(seq_t));
        if (!tmp) {
            if (term_vals) free(term_vals);
            abort();
        }
        term_vals = tmp;
        term_cap = newcap;
    }
    term_vals[term_len++] = (seq_t)val;
}

/* ------------------------------------------------------------------ */
/* Public function required by the interface. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but handle gracefully */

    if (!primes_built) {
        build_prime_table();
    }

    /* Produce terms until we have at least n of them. */
    while (term_len < (size_t)n) {
        if (current_k + 1 >= prime_cnt) {
            /* We ran out of primes – increase the limit and rebuild.
               For this sequence 10 000 000 is ample, but handle just in case. */
            /* Double the current limit and repeat. */
            size_t old_cnt = prime_cnt;
            size_t new_limit = (prime_cnt > 0 ? prime_cnt * 2 : 100000);   /* crude */
            /* free old data */
            free(prime_vals);
            prime_vals = NULL;
            prime_cnt = 0;
            current_k = 0;
            /* build with larger limit */
            /* Using same sieve but larger */
            /* We reuse build_prime_table logic with a dynamic limit */
            /* For simplicity, abort if this rarely happens */
            abort();
        }

        uint64_t p1 = prime_vals[current_k];
        uint64_t p2 = prime_vals[current_k + 1];
        uint64_t sum = p1 + p2 + 1;          /* always < 2×10⁷ with our limit */

        if (is_prime_u64(sum)) {
            append_term((seq_t)sum);
        }
        current_k++;
    }

    return term_vals[n - 1];
}
