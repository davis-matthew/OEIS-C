/*
 * A178623  Triangle T(n,m) read by rows: T(n,0)= prime(n); T(n,m)=1 if m>=1.
 * https://oeis.org/A178623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A178623: 1, 2,1, 3,1,1, 5,1,1,1,1, 7,1,1,1,1,1,1, ...
 *  The sequence is the triangle read by rows where
 *  T(n,0) = prime(n)   (with prime(0)=1)
 *  T(n,m) = 1 for 1 ≤ m < prime(n)
 *  a(n) is the element at linear index n when the triangle is flattened.
 *
 *  Implementation: build primes on demand and keep a cumulative sum of
 *  row lengths.  For a given n we find the row r with
 *  cumul_arr[r-1] ≤ n < cumul_arr[r] and output prime(r) if n == cumul_arr[r-1]
 *  else 1.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------  static tables  ---------------------------------------- */
static size_t primes_cnt = 0;          /* number of stored primes (including dummy 1) */
static uint64_t *primes_arr   = NULL; /* primes_arr[0] == 1, primes_arr[1] == 2, ...   */
static uint64_t *cumul_arr   = NULL; /* cumulative lengths of rows */

/* ---------  helper prototypes  ----------------------------------- */
static void init_primes(void);
static void ensure_up_to(idx_t target);
static bool is_prime_candidate(uint64_t x);

/* ---------  initialise tables  ----------------------------------- */
static void init_primes(void)
{
    if (primes_cnt != 0) return;          /* already initialised */

    primes_cnt = 1;                         /* prime[0] = 1  (dummy) */
    primes_arr  = (uint64_t *)malloc(sizeof(uint64_t));
    cumul_arr   = (uint64_t *)malloc(sizeof(uint64_t));

    primes_arr[0] = 1ULL;                  /* prime(0) = 1   */
    cumul_arr[0]  = 1ULL;                  /* first row length = 1 */
}

/* ---------  simple primality test using already known primes ------- */
static bool is_prime_candidate(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 1; i < primes_cnt; ++i) {   /* skip dummy 1 at index 0 */
        uint64_t p = primes_arr[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ---------  generate primes until the cumulative sum exceeds target  */
static void ensure_up_to(idx_t target)
{
    /* Ensure that cumul_arr[primes_cnt-1] > target  */
    while ((idx_t)cumul_arr[primes_cnt - 1] <= target) {
        /* next candidate prime */
        uint64_t cand = primes_arr[primes_cnt - 1] + 1;
        while (!is_prime_candidate(cand)) ++cand;

        /* grow the tables */
        uint64_t *np = (uint64_t *)realloc(primes_arr,
                                           (primes_cnt + 1) * sizeof(uint64_t));
        uint64_t *nc = (uint64_t *)realloc(cumul_arr,
                                           (primes_cnt + 1) * sizeof(uint64_t));
        primes_arr = np;
        cumul_arr = nc;

        primes_arr[primes_cnt] = cand;
        cumul_arr[primes_cnt]  = cumul_arr[primes_cnt - 1] + cand;
        ++primes_cnt;
    }
}

/* ---------  public sequence function -------------------------------- */
seq_t a(idx_t n)
{
    init_primes();
    ensure_up_to(n);

    /* Find the row r such that cumul_arr[r-1] ≤ n < cumul_arr[r]  */
    size_t r = 0;
    while (cumul_arr[r] <= n) ++r;

    idx_t prev_sum = (r == 0) ? 0 : (idx_t)cumul_arr[r - 1];

    if ((idx_t)n == prev_sum)               /* first entry of the row */
        return (seq_t)primes_arr[r];
    else                                     /* all other entries are 1 */
        return (seq_t)1ULL;
}
