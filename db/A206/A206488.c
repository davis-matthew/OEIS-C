/*
 * A206488  The determinant of the distance matrix of the rooted tree having
 *          Matula number n.
 * https://oeis.org/A206488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = determinant of the distance matrix of the rooted tree
 *            with Matula number n. The tree is reconstructed from n according
 *            to the Matula–Goebel definition and the determinant is computed
 *            by a row–pivoted fraction‑free Bareiss algorithm
 * Range:     i8 n <= 8, i16 n <= 44, i32 n <= 60, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* forward declarations of the helper functions */
static void init_primes(void);
static int  build_rec(uint64_t m);

/* global tables for prime handling */
static int  prime_index[10001] = {0};
static bool primes_initialized = false;

/* prime table initialization */
static void init_primes(void)
{
    if (primes_initialized) return;
    const int LIMIT = 10000;
    bool sieve[LIMIT + 1];
    for (int i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= LIMIT; j += i) sieve[j] = false;
    int idx = 0;
    for (int i = 2; i <= LIMIT; ++i)
        if (sieve[i]) prime_index[i] = ++idx;
    primes_initialized = true;
}

/* recursive construction of the tree from its Matula number.
   The function returns the index of the constructed root vertex.
   Global arrays `edges` and `edgeCount` record the edges of the tree.
   `nextVertex` is a counter for assigning unique vertex indices. */
static int nextVertex;
static int edgeCount;
static int edges[64][2];
static int build_rec(uint64_t m)
{
    int cur = nextVertex++;
    if (m == 1) return cur;                /* single vertex tree */

    uint64_t temp = m;
    while (temp > 1) {
        uint64_t p = 0;
        for (uint64_t i = 2; i * i <= temp; ++i) {
            if (temp % i == 0) { p = i; break; }
        }
        if (p == 0) p = temp;              /* temp itself is prime */

        int childMat = prime_index[(int)p]; /* index of prime p */
        while (temp % p == 0) {
            temp /= p;
            int child = build_rec(childMat);
            edges[edgeCount][0] = cur;
            edges[edgeCount][1] = child;
            ++edgeCount;
        }
    }
    return cur;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* initialize prime tables only once */
    init_primes();

    /* reset per-call data */
    nextVertex = 0;
    edgeCount = 0;

    /* build the tree; root vertex index returned but not needed */
    (void)build_rec((uint64_t)n);
    const int V = nextVertex;          /* number of vertices */

    /* adjacency lists (max V is small, ≤ 64 for n ≤ 10000) */
    int adj[64][64] = {0};             /* neighbors */
    int adjCnt[64] = {0};

    for (int i = 0; i < edgeCount; ++i) {
        int u = edges[i][0];
        int v = edges[i][1];
        adj[u][adjCnt[u]++] = v;
        adj[v][adjCnt[v]++] = u;
    }

    /* compute all‑pairs distances by BFS (edge weight 1) */
    int dist[64][64];
    for (int s = 0; s < V; ++s) {
        for (int t = 0; t < V; ++t) dist[s][t] = -1;
        int queue[64];
        int qh = 0, qt = 0;
        queue[qt++] = s;
        dist[s][s] = 0;
        while (qh < qt) {
            int u = queue[qh++];
            for (int k = 0; k < adjCnt[u]; ++k) {
                int v = adj[u][k];
                if (dist[s][v] == -1) {
                    dist[s][v] = dist[s][u] + 1;
                    queue[qt++] = v;
                }
            }
        }
    }

    /* special case: single‑vertex tree has determinant 0 */
    if (V == 1) return (seq_t)0;

    /* Matrix for Bareiss determinant (use seq_t for all entries) */
    seq_t M[64][64];
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            M[i][j] = (seq_t)dist[i][j];

    int sign = 1;                 /* sign accumulated from row swaps */

    for (int k = 0; k < V - 1; ++k) {
        if (M[k][k] == (seq_t)0) {              /* pivot = 0 -> swap rows */
            int r = -1;
            for (int i = k + 1; i < V; ++i)
                if (M[i][k] != (seq_t)0) { r = i; break; }
            if (r == -1) return (seq_t)0;      /* singular, determinant 0 */

            /* swap rows k and r */
            for (int c = 0; c < V; ++c) {
                seq_t tmp = M[k][c];
                M[k][c] = M[r][c];
                M[r][c] = tmp;
            }
            sign = -sign;
        }

        seq_t pivot = M[k][k];
        seq_t denom = (k == 0 ? (seq_t)1 : M[k - 1][k - 1]);

        for (int i = k + 1; i < V; ++i)
            for (int j = k + 1; j < V; ++j) {
                seq_t num = M[i][j] * pivot - M[i][k] * M[k][j];
                M[i][j] = num / denom;          /* exact division */
            }
    }

    seq_t det = M[V - 1][V - 1];
    if (sign < 0) det = -det;
    return det;
}
