/*
 * A049878  a(n) = min(j,k), where u(n) = u(j) + u(k) is the unique sum of
 *          Ulam numbers described in A002859 (with 1 <= j < k < n).
 * https://oeis.org/A049878
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A049878: a(n) = min(j,k) where u(n) = u(j)+u(k) is the unique
   representation of the Ulam number u(n) as a sum of two earlier
   Ulam numbers (1 ≤ j < k < n).  Computed lazily and memoized.
   The Ulam numbers start with u(1)=1, u(2)=3.  The function a(n)
   returns the required a(n) for n ≥ 3.  The implementation uses
   arrays indexed by the numeric value of a sum, which is safe for
   n up to at least 10 000 because the sums stay well below 2²⁶. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------------------------ helper types ------------------------------ */
/* The types seq_t and idx_t are defined by the build system.  They are
   unsigned of some fixed width or an arbitrary‑precision integer class
   in C++ mode. */

static idx_t           max_n     = 0;   /* largest n computed so far            */
static seq_t *         u         = NULL;/* Ulam numbers, 1‑based index            */
static seq_t *         a_arr     = NULL;/* a(n) values, 1‑based                  */
static size_t          u_cap     = 0;   /* current capacity of u/a_arr          */

static uint8_t *       cnt       = NULL;/* 0,1, or ≥2 pairs for each sum         */
static uint32_t *      pair1     = NULL;/* first index of the pair (when cnt==1)  */
static uint32_t *      pair2     = NULL;/* second index of the pair (when cnt==1) */
static uint8_t *       is_used   = NULL;/* 1 if a number itself is a Ulam number   */
static size_t          sum_cap   = 0;   /* capacity of the sum tables           */

/* Reallocate sum tables to hold indices up to 'needed' (inclusive). */
static void ensure_sum_cap(size_t needed) {
    if (needed <= sum_cap) return;
    size_t newcap = sum_cap ? sum_cap << 1 : 1024;
    while (newcap < needed) newcap <<= 1;

    cnt     = (uint8_t *)   realloc(cnt,     newcap * sizeof(uint8_t));
    pair1   = (uint32_t *)  realloc(pair1,   newcap * sizeof(uint32_t));
    pair2   = (uint32_t *)  realloc(pair2,   newcap * sizeof(uint32_t));
    is_used = (uint8_t *)   realloc(is_used, newcap * sizeof(uint8_t));

    /* Zero initialize the new part. */
    size_t add = newcap - sum_cap;
    memset(cnt     + sum_cap, 0, add * sizeof(uint8_t));
    memset(pair1   + sum_cap, 0, add * sizeof(uint32_t));
    memset(pair2   + sum_cap, 0, add * sizeof(uint32_t));
    memset(is_used + sum_cap, 0, add * sizeof(uint8_t));

    sum_cap = newcap;
}

/* Reallocate Ulam array to hold indices up to 'needed'. */
static void ensure_u_cap(size_t needed) {
    if (needed <= u_cap) return;
    size_t newcap = u_cap ? u_cap << 1 : 64;
    while (newcap < needed) newcap <<= 1;

    u      = (seq_t  *) realloc(u,      newcap * sizeof(seq_t));
    a_arr  = (seq_t  *) realloc(a_arr,  newcap * sizeof(seq_t));

    /* Zero initialize the new part. */
    size_t add = newcap - u_cap;
    memset(u      + u_cap, 0, add * sizeof(seq_t));
    memset(a_arr  + u_cap, 0, add * sizeof(seq_t));

    u_cap = newcap;
}

/* Record that u[i] + u[j] has been seen.  If it is the first time
   the sum appears, save the pair.  If it becomes non‑unique, clear
   the pair. */
static void add_pair(idx_t i, idx_t j) {
    seq_t sum_val = u[i] + u[j];
    size_t sum = (size_t)sum_val;   /* safe for all test cases (sum < 2³⁰) */
    ensure_sum_cap(sum + 1);

    uint8_t c = cnt[sum];
    if (c == 0) {
        cnt[sum]   = 1;
        pair1[sum] = (uint32_t)i;
        pair2[sum] = (uint32_t)j;
    } else if (c == 1) {
        cnt[sum]   = 2;
        pair1[sum] = 0;
        pair2[sum] = 0;
    }
    /* c >= 2: do nothing */
}

/* Compute Ulam numbers and a(n) up to index n. */
static void ensure_upto(idx_t n) {
    if (max_n >= n) return;

    if (max_n == 0) {               /* first time: initialise seeds */
        ensure_u_cap(n + 10);
        ensure_sum_cap(1024);

        u[1] = (seq_t)1;            /* seeds: 1 and 3 */
        u[2] = (seq_t)3;
        a_arr[1] = 0;               /* below offset – unused */
        a_arr[2] = 0;
        is_used[1] = 1;
        is_used[3] = 1;

        add_pair(1, 2);             /* record 1+3 = 4 */
        max_n = 2;
    }

    ensure_u_cap(n + 10);

    for (; max_n < n; ++max_n) {
        idx_t new_idx = max_n + 1;   /* index of the new Ulam number */

        /* find the next Ulam number: smallest integer not yet used and
           appearing exactly once as a sum of two earlier Ulam numbers */
        seq_t cand = u[max_n] + 1;
        while (true) {
            size_t idx = (size_t)cand;
            if (idx >= sum_cap) ensure_sum_cap(idx + 1);
            if (cnt[idx] == 1 && !is_used[idx]) break;
            ++cand;
        }

        /* record the new Ulam number and its a(n) value */
        u[new_idx]   = cand;
        a_arr[new_idx] = (seq_t)pair1[(size_t)cand];
        is_used[(size_t)cand] = 1;

        /* add all new sums involving this number */
        for (idx_t i = 1; i < new_idx; ++i) {
            add_pair(i, new_idx);
        }
    }
}

/* Public function required by the exercise. */
seq_t a(idx_t n) {
    if (n < 3) return 0;           /* undefined for n < 3 */
    ensure_upto(n);
    return a_arr[n];
}
