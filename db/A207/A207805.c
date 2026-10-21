/*
 * A207805  Number of nX5 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 0 1 vertically.
 * https://oeis.org/A207805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed as number of n×5 binary arrays that avoid
 *            horizontally 0 0 0 and 0 0 1, and vertically 0 1 0 and 1 0 1
 *            patterns. The sequence satisfies the recurrence given in the
 *            OEIS entry
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   row_allowed, triple_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) is defined below. */
/* It uses a transfer‑matrix DP with states of two rows. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/row_allowed.h"
#include "../utils/triple_allowed.h"

static void init(void);

static int allowed_masks[16];
static int mask_to_idx[32];
static int succ_next[256][16];
static unsigned char succ_cnt[256];

static seq_t dp_state[2][256];
static int dp_cur = 0;

/* dynamic cache for a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_computed = 0; /* largest n computed and stored */

/* initialise global tables: allowed row masks, transition lists */
static void init(void)
{
    int cnt = 0;
    for (int m = 0; m < 32; ++m) {
        mask_to_idx[m] = -1;
    }
    for (int m = 0; m < 32; ++m) {
        if (row_allowed(m)) {
            allowed_masks[cnt] = m;
            mask_to_idx[m] = cnt;
            ++cnt;
        }
    }
    /* K should be 16 */
    for (int s1 = 0; s1 < 256; ++s1) {
        succ_cnt[s1] = 0;
    }
    for (int idx1 = 0; idx1 < 16; ++idx1) {
        for (int idx2 = 0; idx2 < 16; ++idx2) {
            int s1 = idx1 * 16 + idx2;
            for (int idx3 = 0; idx3 < 16; ++idx3) {
                if (triple_allowed(allowed_masks[idx1],
                                   allowed_masks[idx2],
                                   allowed_masks[idx3])) {
                    int s2 = idx2 * 16 + idx3;
                    succ_next[s1][succ_cnt[s1]++] = s2;
                }
            }
        }
    }
    /* initialise dp_state for length 2: all state counts = 1 */
    for (int s = 0; s < 256; ++s) dp_state[0][s] = 1;
    dp_cur = 0;
    cache_computed = 2; /* a(1) and a(2) are known */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }
    cache[0] = 16;          /* a(1) */
    cache[1] = 256;        /* a(2) */
}

/* The public function */
seq_t a(idx_t n)
{
    static int init_done = 0;
    if (!init_done) {
        init_done = 1;
        init();
    }

    if (n == 0) return 0; /* undefined for n<1, return 0 */

    if (n <= cache_computed) {
        return cache[n - 1];
    }

    /* Ensure cache big enough */
    if (n > cache_cap) {
        size_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute from the current computed value up to n */
    for (idx_t len = cache_computed + 1; len <= n; ++len) {
        /* compute dp_next for this length */
        int next_state = 1 - dp_cur;
        /* zero dp_next */
        for (int s = 0; s < 256; ++s) {
            dp_state[next_state][s] = 0;
        }
        /* transition */
        for (int s1 = 0; s1 < 256; ++s1) {
            seq_t val = dp_state[dp_cur][s1];
            if (val == 0) continue;
            unsigned char ccount = succ_cnt[s1];
            for (unsigned char k = 0; k < ccount; ++k) {
                int s2 = succ_next[s1][k];
                dp_state[next_state][s2] = dp_state[next_state][s2] + val;
            }
        }
        /* sum to get a(len) */
        seq_t total = 0;
        for (int s = 0; s < 256; ++s) {
            total = total + dp_state[next_state][s];
        }
        cache[len - 1] = total;

        dp_cur = next_state;
    }
    cache_computed = n;
    return cache[n - 1];
}
