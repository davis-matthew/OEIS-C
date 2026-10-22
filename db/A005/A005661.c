/*
 * A005661  k in S implies 2k-2, 3k-3 in S.
 * https://oeis.org/A005661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 3586, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A005661_c2, A005661_c3, alloc_array, grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: S = closure of {5} under k -> 2k-2, 3k-3; merge the two image streams in increasing order (as for Hamming numbers), with duplicates removed */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/alloc_array.h"
#include "../utils/grow_arr.h"
#include "../utils/A005661_c2.h"
#include "../utils/A005661_c3.h"

static seq_t *A005661_t = 0;
static idx_t A005661_len = 0, A005661_cap = 0;
static idx_t A005661_i2 = 0, A005661_i3 = 0;
static bool A005661_stuck = false;

seq_t a(idx_t n)
{
    if (A005661_t == 0) {
        A005661_cap = 64;
        A005661_t = alloc_array(A005661_cap);
        A005661_t[0] = 5;
        A005661_len = 1;
    }
    while (A005661_len < n && !A005661_stuck) {
        seq_t v2 = 0, v3 = 0, m;
        bool ok2 = A005661_c2(A005661_t[A005661_i2], &v2);
        bool ok3 = A005661_c3(A005661_t[A005661_i3], &v3);
        if (!ok2 && !ok3) { A005661_stuck = true; break; }
        if (ok2 && (!ok3 || v2 <= v3)) m = v2; else m = v3;
        if (ok2 && v2 == m) A005661_i2++;
        if (ok3 && v3 == m) A005661_i3++;
        if (A005661_len == A005661_cap) {
            idx_t nc = A005661_cap * 2;
            A005661_t = grow_arr(A005661_t, A005661_cap, nc);
            A005661_cap = nc;
        }
        A005661_t[A005661_len++] = m;
    }
    if (n > A005661_len) return A005661_t[A005661_len - 1];
    return A005661_t[n - 1];
}
