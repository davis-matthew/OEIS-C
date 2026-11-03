/*
 * A207324  List of permutations of 1,2,3,...,n for n=1,2,3,..., in the order
 *          they are output by Steinhaus-Johnson-Trotter algorithm.
 * https://oeis.org/A207324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = element at position n in the infinite sequence obtained
 *            by concatenating all Steinhaus‑Johnson‑Trotter permutations of
 *            {1,…,k} for k = 1,2,3,… (1‑based index)
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9900 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Return the element at index `pos` (0‑based) of the
   `perm_idx`‑th (0‑based) Steinhaus‑Johnson‑Trotter permutation
   of size `k`. (`k` ≤ 25). */
static seq_t kth_perm_elem(int k, idx_t perm_idx, int pos)
{
    int perm[25];
    int dir[25];

    for (int i = 0; i < k; ++i) {
        perm[i] = i + 1;
        dir[i]  = -1;      /* direction: -1 = left, +1 = right */
    }

    for (idx_t step = 0; step < perm_idx; ++step) {
        /* Find largest mobile element. */
        int max_mobile = 0, max_pos = -1;
        for (int i = 0; i < k; ++i) {
            int neigh = i + dir[i];
            if (neigh < 0 || neigh >= k) continue;
            if (perm[i] > perm[neigh] && perm[i] > max_mobile) {
                max_mobile = perm[i];
                max_pos    = i;
            }
        }

        /* Swap the mobile element with the adjacent element. */
        int neigh = max_pos + dir[max_pos];
        int tmp = perm[max_pos];
        perm[max_pos] = perm[neigh];
        perm[neigh]   = tmp;

        /* Swap the directions as well, to keep them attached to the elements. */
        int tmpd = dir[max_pos];
        dir[max_pos] = dir[neigh];
        dir[neigh]   = tmpd;

        /* Reverse directions of all elements larger than the moved one. */
        for (int i = 0; i < k; ++i) {
            if (perm[i] > max_mobile) dir[i] = -dir[i];
        }
    }

    return (seq_t)perm[pos];
}

/* Pre‑compute factorials up to 25 (more than enough for n up to 2^64−1). */
static seq_t fact_cache[26];
static int fact_ready = 0;

static void init_fact(void)
{
    if (fact_ready) return;
    fact_cache[0] = (seq_t)1;
    for (int i = 1; i <= 25; ++i) {
        fact_cache[i] = fact_cache[i-1] * (seq_t)i;
    }
    fact_ready = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* defined only for n ≥ 1 */

    init_fact();

    /* Find the block size k such that (k+1)! - 1 ≥ n.
       The length of block k is fact_cache[k] * k.            */
    seq_t sum = 0;
    int k = 1;
    seq_t block_len = 0;

    for (;;) {
        if (k > 25) {            /* should never happen for valid n */
            return 0;
        }
        block_len = fact_cache[k] * (seq_t)k;
        sum += block_len;
        if ((seq_t)n <= sum) break;
        ++k;
    }

    seq_t prev_total = sum - block_len;
    seq_t offset0 = (seq_t)n - prev_total - (seq_t)1;   /* 0‑based offset in block */

    idx_t perm_idx = (idx_t)(offset0 / (seq_t)k);
    int    pos     = (int)(offset0 % (seq_t)k);

    return kth_perm_elem(k, perm_idx, pos);
}
