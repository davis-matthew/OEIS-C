/*
 * A004050  Numbers of the form 2^j + 3^k, for j and k >= 0.
 * https://oeis.org/A004050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest distinct number of the form 2^j + 3^k, j,k
 *            >= 0. Search: list every 2^j + 3^k <= B, sort it, remove
 *            duplicates, and double the exponent of B = 2^E until at least n
 *            terms are found
 * Range:     u8 n <= 40, u16 n <= 168, u32 n <= 665, u64 n <= 2618,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_zero, release, shell_sort
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/shell_sort.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t cur_e = 0;
static int done_full = 0;

static idx_t gen(seq_t *out, seq_t bound, int full) {
    idx_t cnt = 0;
    seq_t p2 = 1;
    for (;;) {
        if (!full && p2 > bound) break;
        seq_t p3 = 1;
        for (;;) {
            if (!full && p3 > bound) break;
            seq_t v = p2 + p3;
            if (v >= p2 && (full || v <= bound)) {
                if (out) out[cnt] = v;
                cnt++;
            }
            seq_t nx = p3 * 3;
            if (nx / 3 != p3) break;
            p3 = nx;
        }
        seq_t nx2 = p2 * 2;
        if (nx2 / 2 != p2) break;
        p2 = nx2;
    }
    return cnt;
}

seq_t a(idx_t n) {
    while (cache_len < n && !done_full) {
        cur_e = cur_e ? cur_e * 2 : 8;
        seq_t bound = 1;
        int full = 0;
        for (idx_t i = 0; i < cur_e; i++) {
            seq_t d = bound * 2;
            if (d / 2 != bound) { full = 1; break; }
            bound = d;
        }
        idx_t cnt = gen(NULL, bound, full);
        seq_t *arr = alloc_zero(cnt + 1);
        gen(arr, bound, full);
        shell_sort(arr, cnt);
        idx_t uniq = 0;
        for (idx_t i = 0; i < cnt; i++) {
            if (uniq == 0 || arr[i] != arr[uniq - 1]) {
                arr[uniq] = arr[i];
                uniq++;
            }
        }
        if (cache) release(cache, cache_cap);
        cache = arr;
        cache_cap = cnt + 1;
        cache_len = uniq;
        if (full) done_full = 1;
    }
    if (n >= 1 && n <= cache_len) return cache[n - 1];
    return 0;
}
