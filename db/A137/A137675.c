/*
 * A137675  Prime numbers p such that p +- a, p +- b, p +- c and p +- d are
 *          prime numbers, where a, b, c and d are distinct positive integers
 *          with p > d, d > c, c > b and b > a.
 * https://oeis.org/A137675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that they have at least four symmetric
   prime offsets.  The computation is done by maintaining a sieve
   of primes up to 2*max_search and testing each prime found.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ----------   global sieve state   ---------- */
static bool *prime_flag = NULL;            /* prime_flag[i] == true <=> i is prime */
static size_t prime_flag_limit = 0;        /* array size   */
static uint32_t *prime_list = NULL;        /* list of primes in ascending order */
static size_t prime_count = 0;             /* number of primes in prime_list */

/* ----------   cache of qualifying primes   ---------- */
static seq_t *qual_primes = NULL;          /* array holding the qualifying primes */
static size_t qual_size   = 0;             /* number of elements stored    */
static size_t qual_cap    = 0;             /* current capacity              */

/* ----------   current search state   ---------- */
static size_t max_prime_search = 0;        /* highest prime examined so far */
static size_t prime_idx = 0;              /* index of next prime to test  */

/* ----------   build or enlarge sieve   ---------- */
static void build_sieve(size_t limit)
{
    if (limit <= prime_flag_limit)
        return;   /* already built up to at least 'limit' */

    /* allocate new flag array */
    bool *new_flags = (bool *)calloc(limit + 1, sizeof(bool));
    if (!new_flags) exit(1);
    for (size_t i = 2; i <= limit; ++i)
        new_flags[i] = true;
    new_flags[0] = new_flags[1] = false;

    size_t sq = (size_t)sqrt((double)limit);
    for (size_t i = 2; i <= sq; ++i) {
        if (new_flags[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                new_flags[j] = false;
        }
    }

    /* count primes to know list size */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (new_flags[i]) ++cnt;

    /* allocate new list */
    uint32_t *new_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!new_list) {
        free(new_flags);
        exit(1);
    }
    size_t pos = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (new_flags[i]) new_list[pos++] = (uint32_t)i;

    /* replace old data */
    free(prime_flag);
    free(prime_list);
    prime_flag = new_flags;
    prime_flag_limit = limit;
    prime_list = new_list;
    prime_count = cnt;
}

/* ----------   count symmetric prime offsets for a prime   ---------- */
static int count_sym_offsets(uint32_t p, size_t p_idx)
{
    int cnt = 0;
    for (size_t j = 0; j < p_idx && cnt < 4; ++j) {
        uint32_t q = prime_list[j];
        uint32_t d = p - q;                /* d > 0 */
        if (prime_flag[p + d])             /* check upper side */
            ++cnt;
    }
    return cnt;
}

/* ----------   sequence a(n)   ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, return 0 */

    /* initialise on first call */
    if (max_prime_search == 0) {
        max_prime_search = 1000;                    /* start with a small limit   */
        build_sieve(max_prime_search * 2);
    }

    /* grow the cached sequence until we have at least n elements */
    while (qual_size < (size_t)n) {
        /* ensure we have enough primes to examine */
        if (prime_idx >= prime_count ||
            prime_list[prime_idx] > max_prime_search) {
            /* enlarge the search window */
            max_prime_search *= 2;
            if (max_prime_search < 1000) max_prime_search = 1000;
            build_sieve(max_prime_search * 2);
        }

        if (prime_idx >= prime_count)
            break;  /* safety, should never happen */

        uint32_t p = prime_list[prime_idx];
        size_t p_idx = prime_idx;    /* index of current prime */

        if (p > 3) {  /* primes 2 and 3 cannot have four offsets */
            if (count_sym_offsets(p, p_idx) >= 4) {
                if (qual_size == qual_cap) { /* grow cache */
                    size_t new_cap = qual_cap ? qual_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(qual_primes,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) exit(1);
                    qual_primes = tmp;
                    qual_cap = new_cap;
                }
                qual_primes[qual_size++] = (seq_t)p;
            }
        }

        ++prime_idx;  /* move to next prime */
    }

    return qual_primes[n - 1];
}
