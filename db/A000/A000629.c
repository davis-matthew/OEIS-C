/*
 * A000629  Number of necklaces of partitions of n+1 labeled beads.
 * https://oeis.org/A000629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen, nice
 * Formula:   a(n) = 2*A000670(n) - 0^n, where A000670(n) = Sum_{k=1..n}
 *            T(n,k) with T(n,k) = k*(T(n-1,k-1) + T(n-1,k)), T(0,0) = 1,
 *            T(n,0) = 0 for n >= 1 (ordered set partitions, A019538); a(0) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_arr.h"

static seq_t *row = NULL;   /* row[k] = T(m,k) for the last computed row m */
static seq_t *vals = NULL;  /* vals[i] = a(i) for i < done */
static idx_t done = 0;
static idx_t cap = 0;

static void ensure_cap(idx_t need) {
    idx_t newcap;
    if (cap >= need) return;
    newcap = cap * 2;
    if (newcap < need) newcap = need;
    if (newcap < 64) newcap = 64;
    if (row == NULL) {
        row = alloc_zero(newcap);
        vals = alloc_zero(newcap);
    } else {
        row = grow_arr(row, cap, newcap);
        vals = grow_arr(vals, cap, newcap);
    }
    cap = newcap;
}

seq_t a(idx_t n) {
    ensure_cap(n + 2);
    if (done == 0) {
        row[0] = (seq_t)1;
        vals[0] = (seq_t)1;
        done = 1;
    }
    while (done <= n) {
        idx_t r = done;
        idx_t k;
        seq_t sum = (seq_t)0;
        row[r] = (seq_t)0;
        for (k = r; k >= 1; k--) {
            seq_t t = row[k] + row[k - 1];
            row[k] = (seq_t)k * t;
        }
        row[0] = (seq_t)0;
        for (k = 1; k <= r; k++) {
            sum += row[k];
        }
        vals[r] = sum + sum;
        done++;
    }
    return vals[n];
}
