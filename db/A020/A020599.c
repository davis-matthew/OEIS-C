/*
 * A020599  Smallest nonempty set S containing prime divisors of 5k+7 for each
 *          k in S.
 * https://oeis.org/A020599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = nth prime in the smallest nonempty set S closed under
 *            taking all prime divisors of 5k+7 for k∈S. The set is generated
 *            by starting with 2 and iteratively adding primes dividing 5k+7
 *            for each k processed. The sequence lists the primes of S in
 *            ascending order
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*   Small prime sieve (up to 1 000 000) for trial division factorisation
 * ------------------------------------------------------------------ */
static size_t primes_small_cnt = 0;
static uint32_t primes_small[78498]; /* enough for LIMIT = 1e6 */

static void init_sieve_small(void)
{
    if (primes_small_cnt != 0) return;
    const size_t LIMIT = 1000000;
    bool *is_composite = (bool *)calloc(LIMIT + 1, 1);
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            primes_small[primes_small_cnt++] = (uint32_t)i;
            if (i * i <= LIMIT) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
}

/* ------------------------------------------------------------------ */
/*   Data structures for the closure set (sorted) and frontier
 * ------------------------------------------------------------------ */
static seq_t *set = NULL;          /* sorted array of primes in S            */
static size_t set_cnt = 0;          /* number of elements in set              */
static size_t set_cap = 0;          /* allocated capacity of set              */

static seq_t *frontier = NULL;     /* queue of primes to process            */
static size_t frontier_cnt = 0;     /* total number enqueued                  */
static size_t frontier_cap = 0;     /* allocated capacity of frontier         */
static size_t frontier_pos = 0;     /* current read position in frontier      */

static void init_globals(void)
{
    if (set) return;                 /* already initialised */
    init_sieve_small();

    set_cap = 16;
    set = (seq_t *)malloc(set_cap * sizeof(seq_t));
    set_cnt = 0;

    frontier_cap = 16;
    frontier = (seq_t *)malloc(frontier_cap * sizeof(seq_t));
    frontier_cnt = 0;
    frontier_pos = 0;

    /* start closure with the smallest prime */
    set[set_cnt++] = (seq_t)2;
    frontier[frontier_cnt++] = (seq_t)2;
}

/* ------------------------------------------------------------------ */
/*   Insert a prime into the sorted set if absent.  Append it to
 *   frontier queue for further processing.                          *
 * ------------------------------------------------------------------ */
static void add_prime(uint64_t p)
{
    /* binary search in set */
    size_t lo = 0, hi = set_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (set[mid] < (seq_t)p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* already present? */
    if (lo < set_cnt && set[lo] == (seq_t)p) return;

    /* ensure capacity for set */
    if (set_cnt >= set_cap) {
        size_t ncap = set_cap ? set_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(set, ncap * sizeof(seq_t));
        if (!tmp) abort();
        set = tmp;
        set_cap = ncap;
    }
    /* shift to insert in order */
    for (size_t i = set_cnt; i > lo; --i)
        set[i] = set[i - 1];
    set[lo] = (seq_t)p;
    ++set_cnt;

    /* enqueue to frontier if not already queued */
    if (frontier_cnt >= frontier_cap) {
        size_t ncap = frontier_cap ? frontier_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(frontier, ncap * sizeof(seq_t));
        if (!tmp) abort();
        frontier = tmp;
        frontier_cap = ncap;
    }
    frontier[frontier_cnt++] = (seq_t)p;
}

/* ------------------------------------------------------------------ */
/*   Factor v into prime factors, adding each unique factor to the set
 *   and to the frontier queue.                                        *
 * ------------------------------------------------------------------ */
static void factor_and_process(uint64_t v)
{
    while (v > 1) {
        uint64_t factor = 0;
        /* trial division using small primes */
        for (size_t i = 0; i < primes_small_cnt &&
                           (uint64_t)primes_small[i] * (uint64_t)primes_small[i] <= v; ++i) {
            uint32_t p = primes_small[i];
            if (v % p == 0) {
                factor = p;
                break;
            }
        }
        if (factor == 0)   /* v itself is prime > 1e6 */
            factor = v;

        add_prime(factor);

        while (v % factor == 0)
            v /= factor;
    }
}

/* ------------------------------------------------------------------ */
/*   Main sequence function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined for n ≥ 1 only */

    init_globals();

    /* BFS closure until we have at least n primes and all queued primes
     * have been processed.                                               */
    while ((size_t)n > set_cnt || frontier_pos < frontier_cnt) {
        if (frontier_pos < frontier_cnt) {
            uint64_t k = (uint64_t)frontier[frontier_pos++];
            uint64_t val = 5 * k + 7;
            factor_and_process(val);
        } else {
            /* No more frontier elements but still fewer than n primes
             * – the sequence would terminate before reaching n, which
             * cannot happen for the defined OEIS sequence.             */
            break;
        }
    }

    return set[n - 1];
}
