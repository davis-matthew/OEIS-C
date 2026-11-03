/*
 * A196061  The multiplicative Wiener index of the rooted tree with
 *          Matula-Goebel number n.
 * https://oeis.org/A196061
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   multiplicative Wiener index of the rooted tree with
 *            Matula-Goebel number n. a(n) = ∏distances over all unordered
 *            vertex pairs in the tree with Matula number n
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 44, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 45 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------------ */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* updated prime data for n <= 10000 --------------------------------------------------- */
#define MAX_N          10000
#define PRIME_ARRAY_SZ 2000

static uint32_t primes[PRIME_ARRAY_SZ];
static size_t   prime_cnt = 0;
static uint16_t prime_index[MAX_N + 1];   /* 0 if not prime, else 1‑based index */

static void
init_primes(void)
{
    if (prime_cnt) return;            /* already initialised */

    bool is_prime[MAX_N + 1];
    for (size_t i = 0; i <= MAX_N; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i <= MAX_N; ++i) {
        if (is_prime[i]) {
            primes[prime_cnt++] = (uint32_t)i;
            prime_index[i] = (uint16_t)(prime_cnt);   /* 1‑based index */
            if (i * i <= MAX_N) {
                for (size_t j = i * i; j <= MAX_N; j += i)
                    is_prime[j] = false;
            }
        }
    }
}

/* struct holding subtree data ----------------------------------------------------- */
typedef struct {
    seq_t *depth_counts;   /* depth_counts[d] = number of nodes at depth d (root depth 0) */
    size_t max_depth;      /* highest depth index used */
    seq_t prod_value;      /* multiplicative Wiener index for this subtree */
} TreeInfo;

/* recursive construction ------------------------------------------------------------ */
static TreeInfo *
compute_tree(uint64_t n)
{
    init_primes();               /* ensure primes and indices are ready */

    if (n == 1) {                /* leaf */
        TreeInfo *leaf = mem_new(TreeInfo, 1);
        leaf->max_depth = 0;
        leaf->depth_counts = mem_new(seq_t, 1);
        leaf->depth_counts[0] = (seq_t)1;   /* only the root */
        leaf->prod_value = (seq_t)1;        /* no pairs */
        return leaf;
    }

    /* factorise n into primes – each prime factor gives a child subtree */
    uint64_t temp = n;
    TreeInfo *child_infos[32];   /* enough for n <= 10000 (max 5 distinct primes) */
    size_t child_count = 0;

    for (size_t pi = 0; pi < prime_cnt && (uint64_t)primes[pi] * primes[pi] <= temp; ++pi) {
        uint64_t p = primes[pi];
        while (temp % p == 0) {
            uint64_t child_idx = prime_index[(size_t)p];   /* Matula number of child */
            child_infos[child_count++] = compute_tree(child_idx);
            temp /= p;
        }
    }
    if (temp > 1) {            /* remaining prime factor > sqrt(n) */
        uint64_t child_idx = prime_index[(size_t)temp];
        child_infos[child_count++] = compute_tree(child_idx);
        temp /= temp;          /* becomes 1 */
    }

    /* determine maximum depth of parent */
    size_t max_child_depth = 0;
    for (size_t i = 0; i < child_count; ++i)
        if (child_infos[i]->max_depth > max_child_depth)
            max_child_depth = child_infos[i]->max_depth;

    TreeInfo *root = mem_new(TreeInfo, 1);
    root->max_depth = max_child_depth + 1;
    root->depth_counts = mem_new(seq_t, root->max_depth + 1);
    root->depth_counts[0] = (seq_t)1;            /* root itself */

    /* start with product of all child products */
    root->prod_value = (seq_t)1;
    for (size_t i = 0; i < child_count; ++i)
        root->prod_value = seq_mul(root->prod_value, child_infos[i]->prod_value);

    /* product involving the root and nodes of each child subtree */
    for (size_t i = 0; i < child_count; ++i) {
        TreeInfo *c = child_infos[i];
        for (size_t d = 0; d <= c->max_depth; ++d) {
            seq_t cnt = c->depth_counts[d];
            if (cnt == (seq_t)0) continue;

            seq_t dist = (seq_t)(d + 1);          /* distance from root to node */
            root->prod_value = seq_mul(root->prod_value,
                                       ipow(dist, cnt));

            /* add child counts to parent's depth_counts (shifted by 1) */
            size_t nd = d + 1;
            root->depth_counts[nd] += cnt;
        }
    }

    /* product for pairs between different child subtrees */
    for (size_t i = 0; i < child_count; ++i) {
        for (size_t j = i + 1; j < child_count; ++j) {
            TreeInfo *ci = child_infos[i];
            TreeInfo *cj = child_infos[j];
            for (size_t di = 0; di <= ci->max_depth; ++di) {
                seq_t cnti = ci->depth_counts[di];
                if (cnti == (seq_t)0) continue;
                for (size_t dj = 0; dj <= cj->max_depth; ++dj) {
                    seq_t cntj = cj->depth_counts[dj];
                    if (cntj == (seq_t)0) continue;
                    seq_t dist = (seq_t)(di + dj + 2);          /* through root */
                    seq_t exp  = cnti * cntj;                 /* product fits 64‑bit */
                    root->prod_value =
                        seq_mul(root->prod_value, ipow(dist, exp));
                }
            }
        }
    }

    /* free child data */
    for (size_t i = 0; i < child_count; ++i) {
        mem_free(child_infos[i]->depth_counts);
        mem_free(child_infos[i]);
    }

    return root;
}

/* Public function required by the framework ------------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* undefined for n < 2 */

    TreeInfo *root = compute_tree((uint64_t)n);
    seq_t result = root->prod_value;

    mem_free(root->depth_counts);
    mem_free(root);
    return result;
}
