/*
 * A190175  The Goebel-Matula numbers of the rooted trees having only vertices
 *          of odd degree.
 * https://oeis.org/A190175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: compute OEIS sequence A190175 (Goebel–Matula numbers of rooted
 * trees having only vertices of odd degree).  The function scans natural
 * numbers in increasing order and tests a number n for the property that
 * the rooted tree corresponding to its Matula–Goebel number has no
 * vertex of even degree.  The test is performed recursively using
 * memoisation.  The sequence starts with a(1)=2.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/*--------- global tables for prime sieve and memoisation ---------*/
static uint32_t *spf = NULL;        /* smallest prime factor of i  */
static uint32_t *prime_index = NULL; /* 1‑based index of prime i   */
static uint32_t sieve_limit = 0;
static uint8_t  *root_valid = NULL; /* 0 unknown, 1 true, 2 false */
static uint8_t  *child_valid = NULL;/* 0 unknown, 1 true, 2 false */
static int32_t *sum_exp = NULL;     /* sum of exponents in factorisation of i */ 
static size_t  cache_len = 0;          /* number of terms already generated */
static size_t  cache_cap = 0;
static seq_t *cache = NULL;
static uint32_t next_candidate = 2;

/*--------- helper prototypes -------------------------------------*/
static void ensure_sieve(uint32_t n);
static void init_memo(void);
static bool check_node(uint32_t n, bool is_child);

/*--------- ensure sieve tables are at least up to n ----------------*/
static void ensure_sieve(uint32_t n)
{
    if (n <= sieve_limit) return;
    /* grow limit exponentially */
    uint32_t new_limit = sieve_limit ? sieve_limit * 2 : 1000000;
    while (new_limit < n) new_limit *= 2;

    /* reallocate arrays */
    spf = (uint32_t *)realloc(spf, (new_limit + 1) * sizeof(uint32_t));
    prime_index = (uint32_t *)realloc(prime_index, (new_limit + 1) * sizeof(uint32_t));
    if (!spf || !prime_index) exit(1);

    /* initialise new region */
    for (uint32_t i = sieve_limit + 1; i <= new_limit; ++i) {
        spf[i] = 0;
        prime_index[i] = 0;
    }

    /* linear sieve up to new_limit */
    uint32_t *primes = (uint32_t *)malloc((new_limit / 10 + 1) * sizeof(uint32_t));
    size_t primes_len = 0;
    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[primes_len++] = i;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (prod > new_limit) break;
            spf[prod] = p;
            if (p == spf[i]) break;
        }
    }
    /* fill prime_index */
    for (size_t i = 0; i < primes_len; ++i)
        prime_index[primes[i]] = (uint32_t)(i + 1);
    free(primes);

    /* resize memoisation arrays */
    root_valid = (uint8_t *)realloc(root_valid, (new_limit + 1) * sizeof(uint8_t));
    child_valid = (uint8_t *)realloc(child_valid, (new_limit + 1) * sizeof(uint8_t));
    sum_exp = (int32_t *)realloc(sum_exp, (new_limit + 1) * sizeof(int32_t));
    if (!root_valid || !child_valid || !sum_exp) exit(1);
    for (uint32_t i = sieve_limit + 1; i <= new_limit; ++i) {
        root_valid[i] = 0;
        child_valid[i] = 0;
        sum_exp[i] = -1;
    }
    /* initialise base case */
    if (sieve_limit < 1) {
        root_valid[1] = 0;          /* root not allowed */
        child_valid[1] = 1;         /* leaf is ok as child */
        sum_exp[1] = 0;
    }

    sieve_limit = new_limit;
}

/*--------- memoisation initialisation --------------------------------*/
static void init_memo(void)
{
    if (root_valid) return;
    ensure_sieve(1);
}

/*--------- recursive parity test -------------------------------------*/
static bool check_node(uint32_t n, bool is_child)
{
    if (n == 1) return is_child;  /* leaf: ok only as child */

    init_memo();
    /* return cached value if known */
    if (is_child) {
        if (child_valid[n] == 1) return true;
        if (child_valid[n] == 2) return false;
    } else {
        if (root_valid[n] == 1) return true;
        if (root_valid[n] == 2) return false;
    }

    /* compute sum of exponents */
    int32_t sum = 0;
    uint32_t m = n;
    while (m > 1) {
        uint32_t p = spf[m];
        uint32_t exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
            sum++;
        }
        uint32_t child_idx = prime_index[p];   /* 1‑based index of prime p */
        /* recursively check child as child */
        if (!check_node(child_idx, true)) {   /* child invalid → fail */
            if (is_child) child_valid[n] = 2; else root_valid[n] = 2;
            return false;
        }
        /* the child’s internal root degree (sum of exponents) must be even */
        if (sum_exp[child_idx] % 2 != 0) {
            if (is_child) child_valid[n] = 2; else root_valid[n] = 2;
            return false;
        }
    }

    /* parity of the current node */
    if (is_child) {
        if (sum & 1) {   /* odd -> root degree would be even after parent edge */
            if (child_valid[n]) child_valid[n] = 2;
            return false;
        }
        child_valid[n] = 1;
    } else {
        if ((sum & 1) == 0) {  /* root degree must be odd */
            root_valid[n] = 2;
            return false;
        }
        root_valid[n] = 1;
    }
    sum_exp[n] = sum;
    return true;
}

/*--------- ensure cache capacity ------------------------------------*/
static void ensure_cache_cap(void)
{
    if (cache_len < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    cache_cap = new_cap;
}

/*---------- public sequence function --------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* index is 1‑based; ignore 0 */
    ensure_sieve(2);
    init_memo();
    while (cache_len < n) {
        /* search next qualifying number */
        while (true) {
            if (next_candidate > sieve_limit) ensure_sieve(next_candidate);
            if (check_node(next_candidate, false)) {
                ensure_cache_cap();
                cache[cache_len++] = (seq_t)next_candidate;
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }
    return cache[n - 1];
}
