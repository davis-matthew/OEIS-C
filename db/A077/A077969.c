/*
 * A077969  Numbers which can be expressed as the sum of two distinct primes
 *          in exactly three ways.
 * https://oeis.org/A077969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th even number that can be written as the sum of
 *            two distinct primes in exactly three ways
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We generate primes up to a suitable limit, count for each even
   number how many representations it has, and store the numbers
   whose count equals 3 in an array. The array is extended
   dynamically if a larger index is requested. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*--------- Global state ------------------------------------*/
static idx_t current_limit = 300000;          /* starting sieve limit   */
static idx_t max_needed   = 0;                /* largest n requested   */
static bool   built = false;                  /* have we built data?   */

static int    *prime_list   = NULL;           /* primes up to current_limit   */
static size_t  prime_cnt    = 0;              /* number of primes              */
static unsigned *rep_count = NULL;           /* rep_count[n]   (0..current_limit)   */

static seq_t *term_array = NULL;             /* sequence values   */
static size_t term_cnt  = 0;                 /* number of terms stored   */
static size_t term_cap  = 0;                 /* capacity of term_array    */

/*--------- Utility functions --------------------------------*/
static void free_allocated(void)
{
    free(prime_list);
    free(rep_count);
    free(term_array);
    prime_list   = NULL;
    rep_count    = NULL;
    term_array   = NULL;
    term_cnt     = 0;
    term_cap     = 0;
}

/* Extend term_array if needed */
static void ensure_term_capacity(size_t new_cnt)
{
    if (new_cnt <= term_cap) return;
    size_t new_cap = term_cap ? term_cap * 2 : 4;
    while (new_cap < new_cnt) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(term_array, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);           /* allocation failed */
    term_array = tmp;
    term_cap   = new_cap;
}

/* Build primes, representation counts, and the term list
   up to 'limit'.  All previous arrays are freed and
   replaced by new ones. */
static void build_upto(idx_t limit)
{
    /* free old data */
    free_allocated();

    /* step 1: sieve primes up to limit */
    char *is_prime = (char *)malloc((size_t)(limit + 1));
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= (size_t)limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= (size_t)limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = 0;

    /* collect primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++prime_cnt;
    prime_list = (int *)malloc(prime_cnt * sizeof(int));
    if (!prime_list) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) prime_list[idx++] = (int)i;

    /* step 2: count representations */
    rep_count = (unsigned *)calloc((size_t)(limit + 1), sizeof(unsigned));
    if (!rep_count) exit(1);
    for (size_t a = 0; a < prime_cnt; ++a) {
        int p = prime_list[a];
        for (size_t b = a + 1; b < prime_cnt; ++b) {
            int q = prime_list[b];
            unsigned sum = (unsigned)p + (unsigned)q;
            if (sum > (unsigned)limit) break;
            rep_count[sum]++;
        }
    }
    free(is_prime);

    /* step 3: collect terms with exactly three representations */
    term_cnt = 0;
    term_cap = 0;
    term_array = NULL;
    for (idx_t n = 4; n <= limit; ++n)  /* first even 4 */
        if (rep_count[n] == 3)
            ensure_term_capacity(term_cnt + 1),
            term_array[term_cnt++] = (seq_t)n;
}

/* Ensure we have at least 'need' terms available. */
static void ensure_terms(idx_t need)
{
    if (built && term_cnt >= need) return;
    if (!built) {
        max_needed = need;
        built = true;
    } else if (need > max_needed) {
        max_needed = need;
    }

    /* Keep rebuilding with larger limits until we have enough terms */
    while (term_cnt < need) {
        current_limit *= 2;
        build_upto(current_limit);
    }
}

/*--------- Required function ---------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for 0, return 0 */
    ensure_terms(n);
    return term_array[n - 1];
}
