/*
 * A198334  The Balaban centric index of the rooted tree having Matula-Goebel
 *          number n.
 * https://oeis.org/A198334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the squares of the components of the pruning
 *            partition
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The pruning partition consists of the number of degree‑1 vertices removed at
 *   each successive pruning step.  Here we compute the removal round of each
 *   vertex in the tree with Matula number n, count how many vertices are
 *   removed in each round, then sum the squares of those counts.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_PRIME_LIMIT 5000000  /* prime sieve limit – enough for all needed primes */

/* global prime list */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;

/* adjacency representation of the tree */
static int **adj = NULL;
static size_t *adj_size = NULL;
static size_t *adj_cap = NULL;
static size_t node_cnt = 0;

/* forward declarations */
static idx_t build_tree(idx_t n);
static void add_edge(int u, int v);
static size_t prime_index_of(uint64_t p);
static void sieve_primes(void);

/* ------------------------------------------------------------------------- */
/* Build a prime sieve up to MAX_PRIME_LIMIT                              */
/* ------------------------------------------------------------------------- */
static void sieve_primes(void)
{
    bool *is_composite = (bool*)calloc(MAX_PRIME_LIMIT + 1, sizeof(bool));
    size_t i, j;
    for (i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (!is_composite[i])
            for (j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_composite[j] = true;

    /* count primes */
    prime_cnt = 0;
    for (i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (!is_composite[i]) ++prime_cnt;

    primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (!is_composite[i]) primes[idx++] = (uint32_t)i;

    free(is_composite);
}

/* ------------------------------------------------------------------------- */
/* Find the 1‑based index of prime p.  p must be <= MAX_PRIME_LIMIT          */
/* ------------------------------------------------------------------------- */
static size_t prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* assume p is a prime contained in the sieve */
    return lo + 1;   /* 1‑based */
}

/* ------------------------------------------------------------------------- */
/* Expand the adjacency arrays to a new node                                 */
/* ------------------------------------------------------------------------- */
static void ensure_node_capacity(void)
{
    node_cnt++;                                       /* new node id is node_cnt-1 */
    adj      = (int**)realloc(adj,      node_cnt * sizeof(int*));
    adj_size = (size_t*)realloc(adj_size, node_cnt * sizeof(size_t));
    adj_cap  = (size_t*)realloc(adj_cap,  node_cnt * sizeof(size_t));
    adj[node_cnt-1] = NULL;
    adj_size[node_cnt-1] = 0;
    adj_cap[node_cnt-1]  = 0;
}

/* ------------------------------------------------------------------------- */
/* Add an undirected edge between u and v                                    */
/* ------------------------------------------------------------------------- */
static void add_edge(int u, int v)
{
    /* u -> v */
    if (adj_size[u] == adj_cap[u])
    {
        adj_cap[u] = adj_cap[u] ? adj_cap[u] * 2 : 4;
        adj[u] = (int*)realloc(adj[u], adj_cap[u] * sizeof(int));
    }
    adj[u][adj_size[u]++] = v;

    /* v -> u */
    if (adj_size[v] == adj_cap[v])
    {
        adj_cap[v] = adj_cap[v] ? adj_cap[v] * 2 : 4;
        adj[v] = (int*)realloc(adj[v], adj_cap[v] * sizeof(int));
    }
    adj[v][adj_size[v]++] = u;
}

/* ------------------------------------------------------------------------- */
/* Recursively build the tree for Matula number n                            */
/* ------------------------------------------------------------------------- */
static idx_t build_tree(idx_t n)
{
    ensure_node_capacity();
    int id = (int)(node_cnt - 1);

    if (n == (idx_t)1)
        return (idx_t)id;   /* single vertex tree */

    /* factor n by trial division using the prime list */
    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0) continue;
        size_t k = 0;
        while (temp % p == 0)
        {
            ++k;
            temp /= p;
        }
        /* p is prime – find its Matula index */
        size_t child_index = prime_index_of(p);
        for (size_t t = 0; t < k; ++t)
        {
            idx_t child_id = build_tree((idx_t)child_index);
            add_edge(id, (int)child_id);
        }
    }
    if (temp > 1) /* remaining prime factor */
    {
        size_t child_index = prime_index_of(temp);
        for (;;)
        {
            /* multiply by temp may appear multiple times implicitly via factorization.
             * Since temp is prime and we already have its multiplicity, just add one child.
             */
            idx_t child_id = build_tree((idx_t)child_index);
            add_edge(id, (int)child_id);
            break;
        }
    }

    return (idx_t)id;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function                                                    */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!primes) sieve_primes();

    /* build the tree */
    node_cnt = 0;
    adj   = NULL;
    adj_size = NULL;
    adj_cap  = NULL;
    idx_t root = build_tree(n);

    /* compute removal rounds by BFS from leaves */
    size_t *deg = (size_t*)malloc(node_cnt * sizeof(size_t));
    int *rem_round = (int*)calloc(node_cnt, sizeof(int));
    bool *vis = (bool*)calloc(node_cnt, sizeof(bool));

    size_t max_round = 0;
    /* initialize queue */
    idx_t *q = (idx_t*)malloc(node_cnt * sizeof(idx_t));
    size_t q_h = 0, q_t = 0;

    for (size_t i = 0; i < node_cnt; ++i)
    {
        deg[i] = adj_size[i];
        if (deg[i] <= 1)
        {
            vis[i] = true;
            rem_round[i] = 1;
            q[q_t++] = (idx_t)i;
            if (max_round < 1) max_round = 1;
        }
    }

    while (q_h < q_t)
    {
        int v = (int)q[q_h++];
        for (size_t j = 0; j < adj_size[v]; ++j)
        {
            int u = adj[v][j];
            if (vis[u]) continue;
            if (--deg[u] == 1)
            {
                vis[u] = true;
                rem_round[u] = rem_round[v] + 1;
                if (rem_round[u] > (int)max_round) max_round = rem_round[u];
                q[q_t++] = (idx_t)u;
            }
        }
    }

    /* count frequencies */
    size_t *freq = (size_t*)calloc(max_round + 1, sizeof(size_t));
    for (size_t i = 0; i < node_cnt; ++i)
        freq[rem_round[i]]++;

    /* sum squares of frequencies */
    seq_t sum = 0;
    for (size_t r = 1; r <= max_round; ++r)
    {
        seq_t f = (seq_t)freq[r];
        sum += f * f;
    }

    /* clean up */
    free(deg);
    free(rem_round);
    free(vis);
    free(q);
    free(freq);
    for (size_t i = 0; i < node_cnt; ++i) free(adj[i]);
    free(adj);
    free(adj_size);
    free(adj_cap);
    return sum;
}
