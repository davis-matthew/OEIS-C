/*
 * A233974  Number of (n+1)X(1+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the absolute values of all six edge and diagonal
 *          differences equal to 13.
 * https://oeis.org/A233974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 78*a(n-2) -1896*a(n-4) +18176*a(n-6) -65792*a(n-8)
 *            +67072*a(n-10); Initial values: a(1)=216, a(2)=1308, a(3)=7920,
 *            a(4)=50680, a(5)=323296, a(6)=2103216, a(7)=13644992,
 *            a(8)=89161440, a(9)=581609344, a(10)=3804770496
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Memoisation table for a(n) (0‑based index) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed = 0;            /* number of terms stored in cache */

/* Adjacency matrix for the 49 possible rows (0..6) × (0..6) */
static bool adjacency_ready = false;
static char adj[49][49];

static void init_adjacency(void)
{
    if (adjacency_ready) return;
    for (int s = 0; s < 49; ++s) {
        int a = s / 7;
        int b = s % 7;
        for (int t = 0; t < 49; ++t) {
            int c = t / 7;
            int d = t % 7;
            int sum = abs(a - b) + abs(c - d) +
                      abs(a - c) + abs(b - d) +
                      abs(a - d) + abs(b - c);
            adj[s][t] = (sum == 13);
        }
    }
    adjacency_ready = true;
}

/* Compute the first ten terms by brute‑force DP and store them in the cache */
static void init_sequence(void)
{
    if (cache != NULL) return;            /* already initialised */

    /* Reserve a generous initial capacity */
    cache_cap = 64;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);

    init_adjacency();

    seq_t cur[49], nxt[49];
    for (int i = 0; i < 49; ++i) cur[i] = (seq_t)1;        /* one‑row sequences */

    for (idx_t step = 1; step <= 10; ++step) {
        for (int i = 0; i < 49; ++i) nxt[i] = (seq_t)0;
        for (int s = 0; s < 49; ++s) if (cur[s] != (seq_t)0) {
            for (int t = 0; t < 49; ++t) if (adj[s][t]) {
                nxt[t] += cur[s];
            }
        }

        seq_t total = (seq_t)0;
        for (int i = 0; i < 49; ++i) total += nxt[i];
        cache[computed++] = total;            /* store a(step) */

        /* prepare for next transition */
        for (int i = 0; i < 49; ++i) cur[i] = nxt[i];
    }
}

seq_t a(idx_t n)
{
    init_sequence();

    if (n <= computed) return cache[n-1];

    /* grow cache until it contains a(n) */
    while (computed < n) {
        idx_t k = computed + 1;                 /* 1‑based index of the new term */

        seq_t term;
        if (k <= 10) {                          /* should not happen */
            term = (seq_t)0;
        } else {
            seq_t t1 = (seq_t)78  * cache[k-3];
            seq_t t2 = (seq_t)1896 * cache[k-5];
            seq_t t3 = (seq_t)18176 * cache[k-7];
            seq_t t4 = (seq_t)65792 * cache[k-9];
            seq_t t5 = (seq_t)67072 * cache[k-11];

            term = t1 + t3 + t5;                /* sum of positive terms */
            term -= t2;                         /* subtract negatives */
            term -= t4;
        }

        if (computed >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[computed++] = term;
    }

    return cache[n-1];
}
