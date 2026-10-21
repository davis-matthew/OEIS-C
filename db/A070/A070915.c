/*
 * A070915  Numbers having at most two distinct prime factors.
 * https://oeis.org/A070915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth integer with at most two distinct prime factors
 * (computed by scanning candidates and testing the number of distinct primes).
 * Since no closed formula exists, the program builds the sequence by
 * trial‑division using a growing list of primes.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static bool initialized = false;         /* global init flag */

static uint64_t *prime_list      = NULL; /* array of primes (uint64_t) */
static size_t   prime_cnt      = 0;       /* number of primes stored   */
static size_t   prime_cap      = 0;       /* allocated capacity        */

static seq_t   *seq_vals        = NULL; /* computed sequence terms   */
static size_t   seq_len         = 0;     /* number of terms known     */
static size_t   seq_cap         = 0;     /* allocated capacity        */

static seq_t   cur_candidate    = 1;     /* next integer to test      */

/* ------- helpers ----------------------------------------------------- */

/* ensure the prime list contains all primes up to at least sqrt(val)   */
static void
ensure_primes_for_value(const seq_t val)
{
    /* initialise first prime if needed */
    if (prime_cnt == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[0] = 2;
        prime_cnt = 1;
        return;
    }

    /* generate further primes while the last prime is <= sqrt(val)    */
    uint64_t last = prime_list[prime_cnt - 1];
    while (last <= (uint64_t)(val / last)) {
        uint64_t next = next_prime(last);
        if (prime_cnt == prime_cap) {
            size_t new_cap = prime_cap * 2;
            prime_list = (uint64_t *)realloc(prime_list,
                                             new_cap * sizeof(uint64_t));
            prime_cap = new_cap;
        }
        prime_list[prime_cnt++] = next;
        last = next;
    }
}

/* count distinct prime factors of 'val' using the current prime list   */
static unsigned
count_distinct(const seq_t val)
{
    unsigned cnt = 0;
    seq_t m = val;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];

        /* stop if p*p > m (use division to avoid overflow) */
        if ((seq_t)(m / p) < p)
            break;

        if (m % p == 0) {
            ++cnt;
            while (m % p == 0)
                m /= p;
            if (cnt > 2)
                return cnt;   /* early exit – result no longer needed */
        }
    }
    if (m > 1)                      /* last factor is > sqrt(val) */
        ++cnt;
    return cnt;
}

/* enlarge seq_vals array when needed                                  */
static void
ensure_seq_cap(const size_t required)
{
    if (required <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < required)
        new_cap *= 2;
    seq_vals = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* initialise global state once                                      */
static void
init_once(void)
{
    if (initialized)
        return;
    initialized = true;

    /* primes list – just the first prime to start */
    prime_cap   = 16;
    prime_list  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
    prime_list[0] = 2;
    prime_cnt    = 1;

    /* sequence array */
    seq_cap      = 16;
    seq_vals     = (seq_t *)malloc(seq_cap * sizeof(seq_t));

    cur_candidate = 1;
}

/* ------- required function ------------------------------------------ */

seq_t a(idx_t n)
{
    init_once();

    /* already have enough terms? */
    if ((idx_t)seq_len >= n)
        return seq_vals[n - 1];

    /* generate terms until we reach n */
    while ((idx_t)seq_len < n) {
        ensure_primes_for_value(cur_candidate);

        if (count_distinct(cur_candidate) <= 2) {
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = cur_candidate;
        }
        /* next integer to test                                        */
        cur_candidate += 1;
    }

    return seq_vals[n - 1];
}
