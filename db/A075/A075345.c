/*
 * A075345  Group the natural numbers such that the n-th group contains n
 *          terms and the group sum is the smallest possible prime: (2), (1,
 *          4), (3, 5, 9), (6, 7, 8, 10), (11, 12, 13, 14, 17), (15, 16, 18,
 *          19, 20, 21), ... Sequence gives group sums.
 * https://oeis.org/A075345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) computed by greedy construction of groups, each group n
 *            contains n distinct unused natural numbers, the group sum is the
 *            smallest possible prime; the lexicographically smallest group
 *            achieving that prime is chosen
 * Range:     u8 n <= 7, u16 n <= 49, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

#define GROUP_MAX 10000                   /* maximum n that will be queried */
#define DSU_MAX  50010000ULL              /* safe upper bound for used numbers */
#define DSU_SIZE (DSU_MAX + 2U)           /* array spans indices 0 .. DSU_MAX+1 */

static seq_t group_sums[GROUP_MAX + 1] = {0};   /* 1‑based, ignore index 0 */

static idx_t current_n = 0;                    /* number of groups already computed */
static idx_t next_unused = 1;                  /* local convenience variable */

/* Disjoint‑set structure to find the smallest unused integer efficiently. */
static uint32_t *uf_next = NULL;              /* parent array, 0‑unused */

static void init_union_find(void)
{
    if (!uf_next) {
        size_t sz = DSU_SIZE;
        uf_next = (uint32_t *)calloc(sz, sizeof(uint32_t));
        if (!uf_next) abort();
        for (idx_t i = 0; i <= DSU_MAX + 1; ++i) {
            uf_next[i] = (uint32_t)i;
        }
    }
}

static inline idx_t uf_find(idx_t x)
{
    uint32_t ux = uf_next[x];
    if (ux == x) return x;
    uf_next[x] = (uint32_t)uf_find(ux);
    return uf_next[x];
}

static inline void mark_used(idx_t k)
{
    /* k is now used; link it to the next free integer after k */
    uf_next[k] = (uint32_t)uf_find(k + 1);
}

static inline idx_t smallest_unused(void)
{
    return uf_find(1);
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* sequence offset is 1 */

    init_union_find();                     /* prepare DSU on first call */

    while (current_n < n) {
        ++current_n;                       /* group number (1‑based) */
        idx_t group_size = current_n;
        seq_t sum = 0;

        /* choose the first (group_size‑1) smallest unused integers */
        for (idx_t i = 0; i < group_size - 1; ++i) {
            idx_t num = smallest_unused();
            mark_used(num);
            sum += (seq_t)num;
        }

        /* find the smallest unused integer that makes the sum prime */
        idx_t cand = smallest_unused();
        while (true) {
            seq_t total = sum + (seq_t)cand;
            if (is_prime_big(total)) break;
            cand = uf_find(cand + 1);
        }
        mark_used(cand);
        sum += (seq_t)cand;

        group_sums[current_n] = sum;
    }

    return group_sums[n];
}
