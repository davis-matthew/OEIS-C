/*
 * A061985  Values which can occur in sequence b(k) = 1 + b(floor(k/2)) +
 *          b(floor(k/3)) with b(0) = 0, i.e., in A061984.
 * https://oeis.org/A061985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of A061985: a(n) = nth distinct value of the sequence
   b(k) = 1 + b(floor(k/2)) + b(floor(k/3)), with b(0)=0.
   The function memoises the b(k) values and the sorted distinct
   values, extending them as needed upon each call.
*/
#include <stdlib.h>
#include <string.h>

/* ----------  dynamic storage for b(k) and the distinct set  ---------- */
static seq_t *b = NULL;              /* b(k) values          */
static size_t b_cap = 0, b_len = 0;  /* array capacity/length */

static seq_t *dist = NULL;           /* sorted distinct b(k) */
static size_t dist_cap = 0, dist_len = 0;

/* ensure capacity for the b array */
static void grow_b(size_t needed)
{
    if (needed <= b_cap) return;
    size_t new_cap = b_cap ? b_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    b = tmp;
    b_cap = new_cap;
}

/* ensure capacity for the distinct values array */
static void grow_dist(size_t needed)
{
    if (needed <= dist_cap) return;
    size_t new_cap = dist_cap ? dist_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(dist, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    dist = tmp;
    dist_cap = new_cap;
}

/* insert a new value into the sorted, duplicate‑free array 'dist' */
static void insert_dist(seq_t val)
{
    /* empty list – just add */
    if (dist_len == 0) {
        grow_dist(1);
        dist[0] = val;
        dist_len = 1;
        return;
    }

    /* already present at the end – nothing to do */
    if (val == dist[dist_len-1]) return;

    /* larger than all current – append */
    if (val > dist[dist_len-1]) {
        grow_dist(dist_len + 1);
        dist[dist_len++] = val;
        return;
    }

    /* binary search for insertion point */
    size_t lo = 0, hi = dist_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (dist[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* already present – no insertion needed */
    if (lo < dist_len && dist[lo] == val) return;

    /* shift to make room and insert */
    grow_dist(dist_len + 1);
    memmove(&dist[lo+1], &dist[lo], (dist_len - lo) * sizeof(seq_t));
    dist[lo] = val;
    dist_len++;
}

/* ----------  required function ---------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise on first call */
    if (b_len == 0) {
        grow_b(1);
        b[0] = (seq_t)0;
        b_len = 1;

        grow_dist(1);
        dist[0] = (seq_t)0;
        dist_len = 1;
    }

    /* extend computation until we have at least n+1 distinct values */
    for (size_t k = b_len; dist_len <= n; ++k) {
        grow_b(k + 1);
        seq_t val = (seq_t)1 + b[k / 2] + b[k / 3];
        b[k] = val;
        b_len = k + 1;
        insert_dist(val);
    }

    return dist[n];
}
