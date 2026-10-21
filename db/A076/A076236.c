/*
 * A076236  a(n) = A050435(n) mod A002808(n).
 * https://oeis.org/A076236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(c(n)) mod c(n) where c(k) is the k-th composite number
 *            (c(1)=4)
 * Range:     u8 n <= 1097, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state for memoisation */
static idx_t *prime_arr = NULL;   /* array of primes known so far */
static idx_t *comp_arr  = NULL;   /* array of composite numbers (c(1)=4) */
static idx_t  prime_cap   = 0;
static idx_t  comp_cap    = 0;
static idx_t  prime_cnt   = 0;
static idx_t  comp_cnt    = 0;
static idx_t  next_candidate = 4;   /* next integer to test for primality */

/* initialise static arrays once */
static void init_arrays(void)
{
    if (prime_cnt != 0)   /* already initialised */
        return;

    /* start with primes 2 and 3 */
    prime_cap = 64;
    prime_arr = (idx_t*)malloc(prime_cap * sizeof(idx_t));
    prime_arr[0] = 2;
    prime_arr[1] = 3;
    prime_cnt = 2;

    comp_cap = 64;
    comp_arr = (idx_t*)malloc(comp_cap * sizeof(idx_t));
    comp_cnt = 0;
}

/* enlarge an array if needed */
static void ensure_prime_cap(void)
{
    if (prime_cnt < prime_cap)
        return;
    prime_cap = (prime_cap == 0) ? 64 : prime_cap << 1;
    prime_arr = (idx_t*)realloc(prime_arr, prime_cap * sizeof(idx_t));
}

static void ensure_comp_cap(void)
{
    if (comp_cnt < comp_cap)
        return;
    comp_cap = (comp_cap == 0) ? 64 : comp_cap << 1;
    comp_arr = (idx_t*)realloc(comp_arr, comp_cap * sizeof(idx_t));
}

/* add the next integer to the appropriate list */
static void add_next_candidate(void)
{
    /* primality test using known primes */
    bool is_prime = true;
    for (idx_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_arr[i];
        if (p * p > next_candidate)
            break;
        if (next_candidate % p == 0) {
            is_prime = false;
            break;
        }
    }
    if (is_prime) {
        /* store prime */
        ensure_prime_cap();
        prime_arr[prime_cnt++] = next_candidate;
    } else {
        /* store composite */
        ensure_comp_cap();
        comp_arr[comp_cnt++] = next_candidate;
    }
    ++next_candidate;
}

/* guarantee that comp_arr has at least 'need' composites */
static void ensure_composites_up_to_idx(idx_t need)
{
    init_arrays();
    while (comp_cnt < need) {
        add_next_candidate();
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)  /* behaviour below offset is irrelevant */
        return 0;

    /* first composite c(n) */
    ensure_composites_up_to_idx(n);
    idx_t c_n = comp_arr[n - 1];

    /* index for second application: c(c(n)) */
    idx_t idx_c = (idx_t)c_n;   /* index used to look up the composite */
    /* guard against zero */
    if (idx_c == 0)
        return 0;

    /* composite at that index */
    ensure_composites_up_to_idx(idx_c);
    seq_t c_cn = (seq_t)comp_arr[idx_c - 1];

    return c_cn % (seq_t)c_n;
}
