/*
 * A127284  a(n) = number of valleys (DU-steps) in the Dyck path encoded by
 *          A014486(n).
 * https://oeis.org/A127284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of valleys (down-up) in the Dyck path encoded by
 *            A014486(n)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Dyck path of index n is obtained by the lexicographic unranking of Dyck words. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifndef SEQ_T
/* The build supplies the definition of seq_t and idx_t. */
#endif

/* Maximum semilength needed for n up to about 10000 (Catalan(10) is > 16000). */
#define MAX_K 32

/* DP table for counting completions of a Dyck path given remaining ups, downs and balance. */
static idx_t dp_cnt[MAX_K + 1][MAX_K + 1][MAX_K + 1];
static bool dp_vis[MAX_K + 1][MAX_K + 1][MAX_K + 1];

/* Recursive count of completions. */
static idx_t comp_cnt(int up, int down, int bal)
{
    if (dp_vis[up][down][bal])
        return dp_cnt[up][down][bal];
    idx_t res = 0;
    if (up == 0 && down == 0) {
        res = (bal == 0) ? 1 : 0;
    } else {
        if (up > 0)
            res += comp_cnt(up - 1, down, bal + 1);
        if (down > 0 && bal > 0)
            res += comp_cnt(up, down - 1, bal - 1);
    }
    dp_vis[up][down][bal] = true;
    dp_cnt[up][down][bal] = res;
    return res;
}

/* Unrank the Dyck word of semilength k and local rank r. The result is stored in bits. */
static void build_path(idx_t r, int up, int down, int bal, uint8_t *bits, int *pos)
{
    while (up > 0 || down > 0) {
        if (down > 0 && bal > 0) {
            idx_t cnt0 = comp_cnt(up, down - 1, bal - 1);
            if (r < cnt0) {
                bits[(*pos)++] = 0;
                down -= 1;
                bal -= 1;
                continue;
            }
            r -= cnt0;
        }
        /* choose up */
        bits[(*pos)++] = 1;
        up -= 1;
        bal += 1;
    }
}

seq_t a(idx_t n)
{
    /* Step 1: find the semilength k of the Dyck word at index n. */
    idx_t sum   = 0;
    idx_t C     = 1;     /* Catalan(0) */
    int    k    = 0;
    while (sum + C <= n) {
        sum += C;
        ++k;
        /* Next Catalan number: C_k = C_{k-1} * 2*(2*k-1)/(k+1) */
        C = C * (idx_t)(2 * (2 * k - 1)) / ((k + 1));
        if (k > MAX_K) {
            /* Safety guard; terms beyond MAX_K are not expected for n <= 10000. */
            break;
        }
    }
    /* Local rank within semilength k */
    idx_t r = n - sum;
    if (k == 0)
        return (seq_t)0; /* empty Dyck path has no valleys */

    /* Step 2: unrank the Dyck word */
    uint8_t bits[64];   /* 2*k <= 2*MAX_K <= 64 */
    int pos = 0;
    build_path(r, k, k, 0, bits, &pos);

    /* Step 3: count the valleys (0 followed by 1) */
    seq_t valleys = 0;
    for (int i = 0; i < pos - 1; ++i)
        if (bits[i] == 0 && bits[i + 1] == 1)
            valleys += 1;

    return valleys;
}
