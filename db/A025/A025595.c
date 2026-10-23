/*
 * A025595  Number of n-move king paths on 8 X 8 board from given corner to
 *          any square.
 * https://oeis.org/A025595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of king walks of length n starting at the corner
 *            of an 8×8 board, summed over all destination squares
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *a_cache     = NULL;   /* a_cache[k] = a(k) */
static size_t cache_len   = 0;      /* number of terms cached */
static size_t cache_cap   = 0;      /* allocated capacity of cache */

static seq_t *cur_vec     = NULL;   /* cur_vec[p] = number of walks of current length at square p */
static idx_t cur_n        = 0;      /* current length of cur_vec */

/* adjacency lists for the 8×8 board */
static int  neigh[64][8];
static uint8_t neighcnt[64];
static bool  built = false;

static void build_neigh(void)
{
    if (built) return;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            int id = r * 8 + c;
            int cnt = 0;
            for (int dr = -1; dr <= 1; ++dr) {
                for (int dc = -1; dc <= 1; ++dc) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr;
                    int nc = c + dc;
                    if (nr < 0 || nr >= 8 || nc < 0 || nc >= 8) continue;
                    neigh[id][cnt++] = nr * 8 + nc;
                }
            }
            neighcnt[id] = cnt;
        }
    }
    /* initialize base vector cur_vec for n=0 */
    cur_vec = (seq_t *)calloc(64, sizeof(seq_t));
    cur_vec[0] = 1;                     /* corner (0,0) */
    /* initialize cache with a(0) */
    cache_cap = 16;
    a_cache   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 1;
    a_cache[0] = 1;
    cur_n = 0;
    built = true;
}

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;   /* already cached */

    if (n >= cache_cap) {
        size_t newcap = cache_cap * 2;
        if (newcap < n + 1) newcap = n + 1;
        a_cache = (seq_t *)realloc(a_cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    /* extend cache up to n */
    seq_t temp[64];
    for (idx_t k = cur_n + 1; k <= n; ++k) {
        /* compute next vector */
        for (int p = 0; p < 64; ++p) temp[p] = 0;
        for (int p = 0; p < 64; ++p) {
            int cnt = neighcnt[p];
            for (int j = 0; j < cnt; ++j) {
                int t = neigh[p][j];
                temp[p] += cur_vec[t];
            }
        }
        /* sum over all squares to get a(k) */
        seq_t sum = 0;
        for (int p = 0; p < 64; ++p) sum += temp[p];
        a_cache[k] = sum;

        /* update cur_vec to temp for next iteration */
        for (int p = 0; p < 64; ++p) cur_vec[p] = temp[p];
    }
    cur_n = n;
}

seq_t a(idx_t n)
{
    build_neigh();
    ensure_cache(n);
    return a_cache[n];
}
