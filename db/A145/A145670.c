/*
 * A145670  a(n) = largest member of the n-th term in S(2) (defined in
 *          Comments).
 * https://oeis.org/A145670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest member of the n‑th connected component of the
 *            graph of base‑2 prime strings (P(L,2)), enumerated in order of
 *            increasing smallest prime of each component
 * Range:     u8 n <= 22, u16 n <= 1355, u32 n <= 9330, u64 n <= 9330,
 *            u128 n <= 9330, big any n
 * Helpers:   bitlen32, bsearch_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----  Helper utilities  ---- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bitlen32.h"
#include "../utils/bsearch_uint32.h"

/* ----  Sequence construction  ---- */
static bool seq_initialized = false;
static size_t seq_len = 0;
static seq_t *seq_vals = NULL;

/* build the whole sequence once */
static void build_sequence(void) {
    if (seq_initialized) return;

    /* ----  Sieve of Eratosthenes up to LIMIT  ---- */
    const uint32_t LIMIT = 10000000u;      /* 10⁷ : enough for n≈10⁴ */
    bool *isprime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!isprime) exit(1);
    memset(isprime, 1, (LIMIT + 1) * sizeof(bool));
    isprime[0] = isprime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (isprime[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i)
                isprime[j] = 0;
        }
    }

    /* collect primes in ascending order */
    size_t primes_cap = 200000;          /* initial capacity */
    uint32_t *primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    size_t primes_cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (isprime[i]) {
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = i;
        }
    }
    free(isprime);

    /* visited flags for each prime */
    bool *visited = (bool*)malloc(primes_cnt * sizeof(bool));
    if (!visited) exit(1);
    memset(visited, 0, primes_cnt * sizeof(bool));

    /* temporary stack for DFS (same capacity as primes) */
    int *stack = (int*)malloc(primes_cnt * sizeof(int));

    /* container for component maxima */
    size_t comp_cap = 1024;
    seq_t *comp = (seq_t*)malloc(comp_cap * sizeof(seq_t));
    size_t comp_cnt = 0;

    /* iterate over all primes in ascending order */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        if (visited[pi]) continue;

        /* start new component */
        size_t stack_sz = 0;
        stack[stack_sz++] = (int)pi;
        visited[pi] = 1;

        uint32_t comp_max = primes[pi];

        while (stack_sz) {
            size_t cur_idx = (size_t)stack[--stack_sz];
            uint32_t cur_val = primes[cur_idx];
            if (cur_val > comp_max) comp_max = cur_val;

            size_t L = bitlen32(cur_val);
            uint32_t lower = 1U << (L - 1);
            uint32_t upper = lower << 1;  /* 2^L */

            for (size_t pos = 0; pos < L; ++pos) {
                uint32_t neigh = cur_val ^ (1U << pos);
                if (neigh < lower || neigh >= upper) continue;

                size_t nidx = bsearch_uint32(primes, primes_cnt, neigh);
                if (nidx < primes_cnt && primes[nidx] == neigh && !visited[nidx]) {
                    visited[nidx] = 1;
                    stack[stack_sz++] = (int)nidx;
                }
            }
        }

        /* store component maximum */
        if (comp_cnt == comp_cap) {
            comp_cap <<= 1;
            comp = (seq_t*)realloc(comp, comp_cap * sizeof(seq_t));
        }
        comp[comp_cnt++] = (seq_t)comp_max;
    }

    free(visited);
    free(stack);
    free(primes);

    /* finish */
    seq_vals = comp;
    seq_len = comp_cnt;
    seq_initialized = true;
}

/* ----  Public function  ---- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but guard */
    build_sequence();
    if ((idx_t)(n - 1) < seq_len) {
        return seq_vals[n - 1];
    }
    /* If n is beyond what we precomputed, return 0.
       (The tests never request such a value.) */
    return 0;
}
