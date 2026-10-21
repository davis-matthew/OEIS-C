/*
 * A212621  The overall first Zagreb index of the rooted tree having
 *          Matula-Goebel number n.
 * https://oeis.org/A212621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for A212621:
   a(1) = 0.
   For n>1 let the Matula tree of n have children with
   Matula numbers m1,…,md (obtained from the prime factorisation of n).
   For each child j define
       cj   = number of subtrees of subtree j that contain its root,
       dsumj = sum of degrees of j’s root inside those subtrees,
       sj   = sum of first Zagreb indices of those subtrees.
       aj   = overall first Zagreb index of subtree j.
   Then with these values
       aj = Σ aj (over all children) + 
            Σ_m m²⋅B_m + Σ_j (1+2⋅dsumj)⋅P_j + Σ_j sj⋅P_j
   where  B_m = Σ over subsets of size m of ∏ cj,
          P_j = ∏_{k≠j}(1+c_k).
   The values cj, dsumj and sj for a node are computed in the same
   manner, using the children of that node.  The implementation below
   follows exactly this recurrence and memoises the results for all
   Matula numbers that occur while evaluating a(n).
*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"

struct Info {
    seq_t c;     /* number of subtrees containing this node’s root */
    seq_t dsum;  /* sum of degrees of this root in those subtrees */
    seq_t s;     /* sum of first Zagreb indices of those subtrees */
    seq_t a;     /* overall first Zagreb index of the whole tree */
};

/*---------- Work‑area for primes ----------*/
static idx_t *primes = NULL;          /* array of primes, 1‑based */
static size_t primes_cnt = 0;
static idx_t *prime_index = NULL;     /* prime_index[p] = index of prime p (1‑based), 0 if not prime */

static void init_primes(void) {
    static bool done = false;
    if (done) return;
    idx_t limit = 200000;                 /* enough for all n≤200000 */
    idx_t *primes_raw = NULL;
    size_t prcnt = 0;
    build_primes_upto(limit, &primes_raw, &prcnt);
    primes_cnt = prcnt;
    primes = primes_raw;                  /* primes is idx_t* */
    prime_index = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    memset(prime_index, 0, (limit + 1) * sizeof(idx_t));
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        prime_index[p] = (idx_t)(i + 1);  /* 1‑based index */
    }
    done = true;
}

static inline idx_t get_prime_index(idx_t p) { return prime_index[p]; }

/*---------- Memoisation of Info ----------*/
#define MAX_CHILD 64     /* more than enough for any n up to 200000 */

static struct Info *cache = NULL;    /* array indexed by Matula number */
static size_t cache_size = 0;        /* current allocated size */

static void ensure_cache(size_t n) {
    if (cache_size > n) return;
    size_t new_size = (n + 1) * 2;
    struct Info *new_cache = (struct Info*)realloc(cache, new_size * sizeof(struct Info));
    if (!new_cache) exit(EXIT_FAILURE);
    /* initialise new entries to zero */
    for (size_t i = cache_size; i < new_size; ++i) new_cache[i].c = 0;
    cache = new_cache;
    cache_size = new_size;
}

static struct Info get_info(idx_t n);

/*---------- Factorisation of n into prime factors (with multiplicity) ----------*/
static void factor_into_primes(idx_t n, idx_t *out, size_t *cnt) {
    size_t c = 0;
    idx_t rem = n;
    for (size_t i = 0; i < primes_cnt && (idx_t)(primes[i]) * primes[i] <= rem; ++i) {
        idx_t p = primes[i];
        while (rem % p == 0) {
            out[c++] = p;
            rem /= p;
        }
    }
    if (rem > 1) out[c++] = rem;
    *cnt = c;
}

/*---------- Recursive computation of Info for Matula number n ----------*/
static struct Info get_info(idx_t n) {
    if (n == 1) {                    /* leaf */
        struct Info I = {1,0,0,0};
        return I;
    }

    init_primes();
    ensure_cache(n);
    if (cache[n].c != 0) return cache[n];   /* already computed */

    /* factor n into prime factors (with multiplicity) */
    idx_t factors[64];
    size_t factor_cnt = 0;
    factor_into_primes(n, factors, &factor_cnt);

    idx_t child_indices[64];
    size_t child_cnt = 0;
    for (size_t i = 0; i < factor_cnt; ++i)
        child_indices[child_cnt++] = get_prime_index(factors[i]);

    /* collect Info of each child */
    struct Info child_info[64];
    for (size_t i = 0; i < child_cnt; ++i)
        child_info[i] = get_info(child_indices[i]);

    /* DP arrays (fixed small size) */
    seq_t B[64 + 1];
    seq_t prefix[64 + 1];
    seq_t suffix[64 + 1];
    seq_t prod_without[64];
    seq_t Sum_const = 0;
    seq_t Sum_two_d = 0;
    seq_t Sum_G = 0;
    seq_t sum_m = 0;
    seq_t sum_m2 = 0;
    seq_t sum_child_a = 0;

    /* initialise B zero except B[0]=1 */
    for (size_t i = 0; i <= child_cnt; ++i) B[i] = 0;
    B[0] = 1;

    /* build B: B[m] = Σ over subsets of size m of ∏c_j */
    for (size_t i = 0; i < child_cnt; ++i) {
        seq_t ci = child_info[i].c;
        for (size_t m = child_cnt; m > 0; --m)
            B[m] += B[m-1] * ci;
    }

    /* compute auxiliary sums */
    for (size_t m = 0; m <= child_cnt; ++m) {
        if (m > 0) {
            seq_t mm = (seq_t)m;
            sum_m += B[m] * mm;
            sum_m2 += B[m] * mm * mm;
        }
    }

    seq_t c_total = 0;
    for (size_t m = 0; m <= child_cnt; ++m)
        c_total += B[m];

    /* prefix and suffix for product (1 + c_j) */
    prefix[0] = 1;
    for (size_t i = 0; i < child_cnt; ++i)
        prefix[i+1] = prefix[i] * ((seq_t)1 + child_info[i].c);

    suffix[child_cnt] = 1;
    for (size_t i = child_cnt; i > 0; --i)
        suffix[i-1] = suffix[i] * ((seq_t)1 + child_info[i-1].c);

    /* compute Sum_const, Sum_two_d, Sum_G */
    for (size_t i = 0; i < child_cnt; ++i) {
        seq_t prod = prefix[i] * suffix[i+1];
        seq_t ci = child_info[i].c;
        Sum_const += prod * ci;
        Sum_two_d += (seq_t)2 * child_info[i].dsum * prod;
        Sum_G += child_info[i].s * prod;
    }

    seq_t S_T = sum_m2 + Sum_const + Sum_two_d + Sum_G;

    for (size_t i = 0; i < child_cnt; ++i)
        sum_child_a += child_info[i].a;

    struct Info I = {c_total, sum_m, S_T, sum_child_a + S_T};

    cache[n] = I;                         /* store for future calls */
    return I;
}

/*---------- Public interface ----------*/
seq_t a(idx_t n) {
    struct Info I = get_info(n);
    return I.a;
}
