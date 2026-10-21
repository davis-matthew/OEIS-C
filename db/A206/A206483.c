/*
 * A206483  The matching number of the rooted tree having Matula-Goebel number
 *          n.
 * https://oeis.org/A206483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the maximum matching size of the rooted tree whose
 *            Matula–Goebel number is n. Recurrence (in terms of the two
 *            subproblems for each subtree): dp0[m] – maximum matching of the
 *            tree with root *unmatched* to its children dp1[m] – maximum
 *            matching when the root is matched to exactly one child...
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* ----------------------------------------------------------- */
/* Global tables (static to keep state between calls)          */
/* ----------------------------------------------------------- */
static uint64_t *prime_list      = NULL;   /* primes up to current limit      */
static size_t    prime_cnt       = 0;      /* number of primes                */
static uint32_t *prime_index     = NULL;   /* prime value -> 1‑based index     */
static idx_t    prime_limit      = 0;      /* maximal prime computed so far    */

static seq_t *dp0              = NULL;   /* dp0[m] – root unmatched         */
static seq_t *dp1              = NULL;   /* dp1[m] – root matched to one    */
static unsigned char *computed = NULL;   /* 1 if dp for m already computed */
static idx_t dp_cap = 0;                 /* allocated table size             */

/* ----------------------------------------------------------- */
/* Ensure that prime tables exist up to 'limit'                */
/* ----------------------------------------------------------- */
static void init_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit) return;           /* already ready */

    if (prime_list) { free(prime_list); free(prime_index); }
    build_primes_upto(limit, &prime_list, &prime_cnt);

    /* allocate prime_index array: index p → prime order (1‑based)   */
    prime_index = (uint32_t*)calloc((size_t)limit + 1, sizeof(uint32_t));
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p <= (uint64_t)limit)
            prime_index[p] = (uint32_t)(i + 1);   /* 1‑based index */
    }
    prime_limit = limit;
}

/* ----------------------------------------------------------- */
/* Grow DP tables to hold indices up to 'up_to' (inclusive)    */
/* ----------------------------------------------------------- */
static void ensure_dp_size(idx_t up_to)
{
    if (dp_cap >= up_to) return;

    idx_t new_cap = dp_cap ? dp_cap * 2 : 16;
    while (new_cap < up_to) new_cap <<= 1;

    dp0      = (seq_t*)realloc(dp0, (new_cap + 1) * sizeof(seq_t));
    dp1      = (seq_t*)realloc(dp1, (new_cap + 1) * sizeof(seq_t));
    computed = (unsigned char*)realloc(computed,
                    (new_cap + 1) * sizeof(unsigned char));

    /* initialise newly allocated part as uncomputed */
    for (idx_t i = dp_cap + 1; i <= new_cap; ++i) computed[i] = 0;
    dp_cap = new_cap;
}

/* ----------------------------------------------------------- */
/* Recursive DP computation for Matula number 'm'              */
/* ----------------------------------------------------------- */
static void compute_dp(idx_t m)
{
    if (computed[m]) return;           /* already done */
    ensure_dp_size(m);

    if (m == 1) {                      /* single vertex */
        dp0[1] = 0;
        dp1[1] = 0;
        computed[1] = 1;
        return;
    }

    init_primes_up_to(m);              /* ensure factorisation data */

    /* factorise m into primes, each occurrence gives one child   */
    uint64_t  temp = m;
    idx_t     child[64];
    idx_t     child_cnt = 0;

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= temp; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            while (temp % p == 0) {
                child[child_cnt++] = prime_index[p];
                temp /= p;
            }
        }
    }
    if (temp > 1) {          /* remaining prime factor > sqrt(m) */
        child[child_cnt++] = prime_index[temp];
    }

    /* Compute dp0[m] – best matching when root is unmatched    */
    seq_t sum_all = 0;
    for (idx_t i = 0; i < child_cnt; ++i) {
        idx_t c = child[i];
        compute_dp(c);                 /* ensure child values ready */
        seq_t mxc = dp0[c] > dp1[c] ? dp0[c] : dp1[c];
        sum_all += mxc;
    }

    /* Compute dp1[m] – best matching when root is matched to one child */
    seq_t best_g1 = 0;
    if (child_cnt > 0) {
        for (idx_t i = 0; i < child_cnt; ++i) {
            idx_t c = child[i];
            seq_t g0c  = dp0[c];
            seq_t g1c  = dp1[c];
            seq_t mxc  = g0c > g1c ? g0c : g1c;
            seq_t cand = 1 + g0c + (sum_all - mxc);
            if (cand > best_g1) best_g1 = cand;
        }
    }

    dp0[m] = sum_all;
    dp1[m] = best_g1;
    computed[m] = 1;
}

/* ----------------------------------------------------------- */
/* Public function                                               */
/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* safety (offset starts at 1) */

    ensure_dp_size(n);
    compute_dp(n);
    seq_t ret = dp0[n] > dp1[n] ? dp0[n] : dp1[n];
    return ret;
}
