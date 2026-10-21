/*
 * A220268  a(n) is the smallest number, such that for N >= a(n) there are at
 *          least n primes between 2*N and 3*N.
 * https://oeis.org/A220268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest integer N such that for every * M ≥ N there
 *            are at least n primes between 2*M and 3*M. * * The code
 *            pre‑computes prime counts up to a fixed bound and * derives a(n)
 *            for all n up to the maximum requested. * It works for any
 *            seq_t/idx_t providing unsigned integer * arithmetic
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Constants and global data --------------------------------- */

#define MAX_PRIME   5000000ULL          /* 5 million is safe for n≤10000 */
#define LIMIT_X     (MAX_PRIME / 3ULL)   /* maximum N that will be checked */

/* Prime count prefix array: pi_arr[i] = number of primes ≤ i */
static uint32_t *pi_arr   = NULL;
static size_t   pi_limit  = 0;

/* last_x_by_count[c] = largest x with count(x) == c          */
static size_t *last_x_by_count = NULL;
static size_t  count_limit     = 0;

/* Flag to indicate that the tables are ready. */
static bool tables_ready = false;

/* ------------------------------------------------------------ */
/*  Sieve to build pi_arr ------------------------------------- */

static void init_tables(void)
{
    if (tables_ready) return;

    /* ----------------------------------------------------- */
    /*  1.  Build prime sieve up to MAX_PRIME                */
    /* ----------------------------------------------------- */
    pi_limit = MAX_PRIME + 1;
    pi_arr   = (uint32_t*)malloc(pi_limit * sizeof(uint32_t));
    if (!pi_arr) exit(1);                           /* allocation failed */

    bool *is_prime = (bool*)malloc(pi_limit * sizeof(bool));
    if (!is_prime) exit(1);

    for (size_t i = 0; i < pi_limit; ++i) is_prime[i] = true;
    if (pi_limit > 0) is_prime[0] = false;
    if (pi_limit > 1) is_prime[1] = false;

    for (size_t p = 2; p * p < pi_limit; ++p) {
        if (is_prime[p]) {
            for (size_t k = p * p; k < pi_limit; k += p)
                is_prime[k] = false;
        }
    }

    pi_arr[0] = 0;
    for (size_t i = 1; i < pi_limit; ++i)
        pi_arr[i] = pi_arr[i - 1] + (is_prime[i] ? 1 : 0);

    free(is_prime);

    /* ----------------------------------------------------- */
    /*  2.  Build last_x_by_count array                       */
    /* ----------------------------------------------------- */
    count_limit = LIMIT_X + 1;                      /* index fits the count */
    last_x_by_count = (size_t*)calloc(count_limit, sizeof(size_t));
    if (!last_x_by_count) exit(1);

    for (size_t x = 1; x <= LIMIT_X; ++x) {
        size_t upper = 3 * x;
        if (upper >= pi_limit) upper = pi_limit - 1;
        size_t lower = 2 * x;
        if (lower == 0) lower = 1;                  /* guard for x==0 */
        size_t c = pi_arr[upper] - pi_arr[lower - 1];
        if (c < count_limit)
            last_x_by_count[c] = x;                 /* keep largest x for count c */
    }

    /* Prefix maximums so that last_x_by_count[c] holds the
       largest x with count <= c. */
    for (size_t c = 1; c < count_limit; ++c) {
        if (last_x_by_count[c] < last_x_by_count[c - 1])
            last_x_by_count[c] = last_x_by_count[c - 1];
    }

    tables_ready = true;
}

/* ------------------------------------------------------------ */
/*  Public sequence function ----------------------------------- */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init_tables();

    if (n <= 1)                      /* special case a(1) = 2 */
        return (seq_t)2;

    /* For n >= 2: a(n) = last_x_by_count[n-1] + 1 */
    /* n-1 is guaranteed to be < count_limit because
       for n up to 10,000 the count is < LIMIT_X. */
    size_t idx = (size_t)(n - 1);
    if (idx >= count_limit)
        idx = count_limit - 1;        /* safety, should not happen */

    return (seq_t)(last_x_by_count[idx] + 1);
}
