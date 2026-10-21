/*
 * A235121  Irregular triangle read by rows: row n contains in increasing
 *          order the Matula numbers of the rooted trees that are isomorphic
 *          as trees to the rooted tree with Matula number n (n>=1).
 * https://oeis.org/A235121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: This function enumerates the OEIS sequence A235121.
 * It builds the triangle of Matula numbers by rows, then outputs the
 * nth term in row-major order.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime tables ---------- */
#define PRIME_LIST_SIZE 20000      /* enough for primes up to 200000   */
#define PRIME_LIMIT    200000

static uint32_t prime_list[PRIME_LIST_SIZE];
static size_t   prime_count;
static uint32_t prime_rank[PRIME_LIMIT + 1];

/* initialize prime tables once */
static void init_primes(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    unsigned char is_prime[PRIME_LIMIT + 1] = {0};
    for (int i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = 1;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i) is_prime[j] = 0;

    prime_count = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) {
            prime_list[prime_count] = (uint32_t)i;
            prime_rank[i] = (uint32_t)(prime_count + 1);  /* 1‑based rank */
            ++prime_count;
        }
}

/* get the prime number with 1‑based index */
static inline uint32_t prime_by_index(idx_t idx)
{
    return prime_list[(size_t)idx - 1];
}

/* ---------- tree construction ---------- */
typedef struct {
    size_t *nbrs;
    size_t len;
    size_t cap;
} Node;

typedef struct {
    size_t a, b;
} Edge;

static Edge *edge_list = NULL;
static size_t edge_count = 0;
static size_t edge_cap   = 0;
static size_t vertex_count = 0;

/* append a new edge to the temporary edge list */
static void add_edge_pair(size_t a, size_t b)
{
    if (edge_count == edge_cap) {
        edge_cap = edge_cap ? edge_cap * 2 : 8;
        edge_list = (Edge *)realloc(edge_list, edge_cap * sizeof(Edge));
    }
    edge_list[edge_count].a = a;
    edge_list[edge_count].b = b;
    ++edge_count;
}

/* recursive construction of the tree for Matula number 'mat' */
static size_t build_subtree(uint64_t mat)
{
    size_t cur = vertex_count++;
    uint64_t x = mat;
    while (x > 1) {
        uint32_t spf = 0;
        for (size_t i = 0; i < prime_count &&
                (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= x; ++i) {
            if (x % prime_list[i] == 0) { spf = prime_list[i]; break; }
        }
        if (spf == 0) spf = (uint32_t)x;          /* x itself is prime */
        int cnt = 0;
        while (x % spf == 0) { x /= spf; ++cnt; }
        uint64_t child_mat = prime_rank[spf];
        for (int i = 0; i < cnt; ++i) {
            size_t child_id = build_subtree(child_mat);
            add_edge_pair(cur, child_id);
        }
    }
    return cur;
}

/* build adjacency lists from the temporary edge list */
static void build_tree(uint64_t mat, Node **nodes_out, size_t *vertex_count_out)
{
    vertex_count = 0;
    edge_count   = 0;
    edge_cap     = 0;
    edge_list    = NULL;

    build_subtree(mat);   /* populates edge_list and vertex_count */

    size_t V = vertex_count;
    Node *nodes = (Node *)malloc(V * sizeof(Node));

    /* compute degrees */
    size_t *deg = (size_t *)calloc(V, sizeof(size_t));
    for (size_t i = 0; i < edge_count; ++i) {
        ++deg[edge_list[i].a];
        ++deg[edge_list[i].b];
    }

    for (size_t i = 0; i < V; ++i) {
        nodes[i].cap = deg[i];
        nodes[i].len = 0;
        nodes[i].nbrs = deg[i] ? (size_t *)malloc(deg[i] * sizeof(size_t)) : NULL;
    }

    for (size_t i = 0; i < edge_count; ++i) {
        size_t a = edge_list[i].a;
        size_t b = edge_list[i].b;
        nodes[a].nbrs[nodes[a].len++] = b;
        nodes[b].nbrs[nodes[b].len++] = a;
    }

    free(deg);
    free(edge_list);
    *nodes_out = nodes;
    *vertex_count_out = V;
}

/* ---------- tree DP for Matula numbers ---------- */
static void dfs_down(size_t u, size_t parent, uint64_t *down, size_t *parent_arr,
                     Node *nodes)
{
    parent_arr[u] = parent;
    uint64_t prod = 1;
    for (size_t i = 0; i < nodes[u].len; ++i) {
        size_t v = nodes[u].nbrs[i];
        if (v == parent) continue;
        dfs_down(v, u, down, parent_arr, nodes);
        prod *= (uint64_t)prime_by_index((idx_t)down[v]);
    }
    down[u] = prod;
}

static void dfs_up(size_t u, size_t parent, uint64_t up_u,
                   uint64_t *down, uint64_t *up, Node *nodes)
{
    for (size_t i = 0; i < nodes[u].len; ++i) {
        size_t v = nodes[u].nbrs[i];
        if (v == parent) continue;

        uint64_t prod = 1;
        for (size_t j = 0; j < nodes[u].len; ++j) {
            size_t w = nodes[u].nbrs[j];
            if (w == v) continue;
            uint64_t val = (w == parent) ? up_u : down[w];
            prod *= (uint64_t)prime_by_index((idx_t)val);
        }
        up[v] = prod;
        dfs_up(v, u, prod, down, up, nodes);
    }
}

static void compute_root_mats(size_t V, uint64_t *down, uint64_t *up,
                              size_t *parent_arr, Node *nodes, uint64_t *mats)
{
    for (size_t u = 0; u < V; ++u) {
        uint64_t prod = 1;
        for (size_t i = 0; i < nodes[u].len; ++i) {
            size_t v = nodes[u].nbrs[i];
            uint64_t val = (v == parent_arr[u]) ? up[u] : down[v];
            prod *= (uint64_t)prime_by_index((idx_t)val);
        }
        mats[u] = prod;
    }
}

static void free_nodes(Node *nodes, size_t V)
{
    for (size_t i = 0; i < V; ++i) free(nodes[i].nbrs);
    free(nodes);
}

/* ---------- helper for sorting ---------- */
static int cmp_uint64(const void *p1, const void *p2)
{
    uint64_t a = *(const uint64_t *)p1;
    uint64_t b = *(const uint64_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behavior for n<1 is irrelevant */

    init_primes();          /* ensure prime tables are ready */

    idx_t cumulative = 0;
    size_t row = 1;

    while (true) {
        size_t V, vertex_count_local;
        Node *nodes;
        build_tree((uint64_t)row, &nodes, &vertex_count_local);
        V = vertex_count_local;

        /* allocate DP arrays */
        uint64_t *down = (uint64_t *)malloc(V * sizeof(uint64_t));
        uint64_t *up   = (uint64_t *)malloc(V * sizeof(uint64_t));
        size_t  *parent_arr = (size_t *)malloc(V * sizeof(size_t));

        dfs_down(0, (size_t)-1, down, parent_arr, nodes);
        up[0] = 1;
        dfs_up(0, (size_t)-1, 1, down, up, nodes);

        /* compute all root Matula numbers */
        uint64_t *mats = (uint64_t *)malloc(V * sizeof(uint64_t));
        compute_root_mats(V, down, up, parent_arr, nodes, mats);

        /* sort the values */
        qsort(mats, V, sizeof(uint64_t), cmp_uint64);

        /* count unique values */
        size_t uniqueCount = 0;
        bool first = true;
        uint64_t prev = 0;
        for (size_t i = 0; i < V; ++i) {
            if (first || cmp_uint64(&mats[i], &prev) != 0) {
                ++uniqueCount;
                first = false;
                prev = mats[i];
            }
        }

        if (cumulative + (idx_t)uniqueCount < n) {
            cumulative += (idx_t)uniqueCount;
            free(mats); free(down); free(up); free(parent_arr); free_nodes(nodes, V);
            ++row;
            continue;
        }

        /* target is in this row */
        idx_t offset = n - cumulative;   /* 1‑based position within unique values */

        bool seen_first = true;
        uint64_t last = 0;
        idx_t seen = 0;
        seq_t answer = 0;
        for (size_t i = 0; i < V; ++i) {
            if (seen_first || mats[i] != last) {
                ++seen;
                if (seen == offset) {
                    answer = (seq_t)mats[i];
                    break;
                }
                seen_first = false;
                last = mats[i];
            }
        }

        /* cleanup */
        free(mats); free(down); free(up); free(parent_arr); free_nodes(nodes, V);
        return answer;
    }
}
