/*
 * A006215  Number of down-up permutations of n+6 starting with n+1.
 * https://oeis.org/A006215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T(n+5, n), Entringer numbers: e(0,0) = 1, e(m,0) = 0 for
 *            m > 0, e(m,k) = e(m,k-1) + e(m-1,m-k); a(n) = e(n+5, n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"

static seq_t *ent_row = NULL;   /* current Entringer row e(ent_m, 0..ent_m) */
static idx_t ent_m = 0;
static seq_t *a_cache = NULL;
static idx_t a_len = 0;         /* number of cached terms */
static idx_t a_cap = 0;

seq_t a(idx_t n) {
    if (n < a_len) return a_cache[n];

    if (ent_row == NULL) {
        ent_row = alloc_zero(1);
        ent_row[0] = 1;
        ent_m = 0;
    }
    if (n + 1 > a_cap) {
        idx_t newcap = a_cap ? a_cap : 16;
        while (newcap < n + 1) newcap *= 2;
        if (a_cache == NULL) a_cache = alloc_zero(newcap);
        else a_cache = grow_zero(a_cache, a_cap, newcap);
        a_cap = newcap;
    }

    while (a_len <= n) {
        idx_t target = a_len + 5;
        while (ent_m < target) {
            idx_t m1 = ent_m + 1;
            seq_t *nr = alloc_zero(m1 + 1);
            idx_t k;
            nr[0] = 0;
            for (k = 1; k <= m1; k++)
                nr[k] = nr[k - 1] + ent_row[m1 - k];
            release(ent_row, ent_m + 1);
            ent_row = nr;
            ent_m = m1;
        }
        a_cache[a_len] = ent_row[a_len];
        a_len++;
    }
    return a_cache[n];
}
