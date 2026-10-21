/*
 * A222002  Number of n X 4 arrays with each row a permutation of 1..4 having
 *          at least as many downsteps as the preceding row, with rows in
 *          lexicographically nonincreasing order.
 * https://oeis.org/A222002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence of A222002 a(n) = 11*a(n-1) - 55*a(n-2) +
 *            165*a(n-3) - 330*a(n-4) + 462*a(n-5) - 462*a(n-6) + 330*a(n-7) -
 *            165*a(n-8) + 55*a(n-9) - 11*a(n-10) + a(n-11) This
 *            implementation uses a direct dynamic‑programming approach: for
 *            each n, count sequences of n rows where each row is a p...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 33, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

 /* Forward declaration of dynamic array helpers */
static void ensure_cache_cap(idx_t need);

/* ---------- permutation generation ---------- */
#define PERM_COUNT 24

static int allperm[PERM_COUNT][4];
static unsigned char downcnt[PERM_COUNT];

/* Generate permutations 1..4 in lexicographic ascending order
   recursively.  */
static void gen_perm_rec(int level, int *cur, bool used[5], int *cnt)
{
    if (level == 4) {
        memcpy(allperm[*cnt], cur, 4 * sizeof(int));
        unsigned char d = 0;
        for (int i = 0; i < 3; ++i) {
            if (cur[i] > cur[i + 1]) ++d;
        }
        downcnt[*cnt] = d;
        (*cnt)++;
        return;
    }
    for (int v = 1; v <= 4; ++v) {
        if (!used[v]) {
            used[v] = true;
            cur[level] = v;
            gen_perm_rec(level + 1, cur, used, cnt);
            used[v] = false;
        }
    }
}

static bool perms_initialized = false;
static void init_perms(void)
{
    if (perms_initialized) return;
    int cur[4];
    bool used[5] = { false };
    int cnt = 0;
    gen_perm_rec(0, cur, used, &cnt);
    perms_initialized = true;
}

/* ---------- DP state ---------- */
static seq_t dp[PERM_COUNT];
static seq_t dp_next[PERM_COUNT];
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t current_n = 0;

/* Ensure cache has capacity for 'need' entries */
static void ensure_cache_cap(idx_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? (cache_cap * 2) : 4;
    while (new_cap < need) new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n=0 */

    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* initialise permutations if first call */
    if (!perms_initialized) init_perms();

    /* If this is the very first call, initialise dp to length 1 */
    if (current_n == 0) {
        for (int i = 0; i < PERM_COUNT; ++i) dp[i] = (seq_t)1;
        /* a(1) = sum(dp) = 24 */
        ensure_cache_cap(1);
        seq_t total = 0;
        for (int i = 0; i < PERM_COUNT; ++i) total += dp[i];
        cache[0] = total;
        cache_len = 1;
        current_n = 1;
    }

    /* extend DP to required n */
    for (idx_t step = current_n + 1; step <= n; ++step) {
        /* zero dp_next */
        memset(dp_next, 0, PERM_COUNT * sizeof(seq_t));

        /* transition: for each previous permutation (prev) */
        for (int prev = 0; prev < PERM_COUNT; ++prev) {
            seq_t val = dp[prev];
            unsigned char dp_prev_down = downcnt[prev];
            /* next state indices i can be 0..prev (lex nonincreasing) */
            for (int i = 0; i <= prev; ++i) {
                if (downcnt[i] >= dp_prev_down) {
                    dp_next[i] += val;
                }
            }
        }

        /* copy back */
        memcpy(dp, dp_next, PERM_COUNT * sizeof(seq_t));

        /* compute a(step) = sum(dp) */
        seq_t sum = 0;
        for (int i = 0; i < PERM_COUNT; ++i) sum += dp[i];

        /* store in cache */
        ensure_cache_cap(step);
        cache[step - 1] = sum;
    }

    current_n = n;
    cache_len = n;
    return cache[n - 1];
}
