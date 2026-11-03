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
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 980 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
