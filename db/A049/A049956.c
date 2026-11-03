/*
 * A049956  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 2, and a(3) = 3.
 * https://oeis.org/A049956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)+a(2)+...+a(n-1) + a(m) for n ≥ 4 * where m = 2*n - 3
 *            - 2^(p+1) and p is the unique integer with * 2^p < n-1 ≤
 *            2^(p+1). * Initial values: a(1)=1, a(2)=2, a(3)=3. * The
 *            function memoizes results, so calls may be in any order
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* helper: compute m for index i (i ≥ 4) */
static idx_t compute_m(idx_t i) {
    idx_t t = i - 1;                    /* n-1 */
    int p = 0;
    /* find largest p such that 1<<p < t <= 1<<(p+1) */
    while (((idx_t)1 << (p + 1)) <= t) ++p;
    if (((idx_t)1 << p) == t) --p;      /* adjust if t is exact power of two */
    idx_t power = ((idx_t)1 << (p + 1));
    return 2 * i - 3 - power;
}

static seq_t *arr = NULL;    /* values a[n]  (index 1..len) */
static seq_t *pref = NULL;  /* prefix sums S[n] = a(1)+...+a(n)  (index 0..len) */
static size_t cap = 0;      /* allocated capacity (>= len+1) */
static size_t len = 0;      /* highest index computed */

static void ensure_capacity(size_t need) {
    if (cap >= need) return;
    size_t newcap = cap ? cap << 1 : 16;
    while (newcap < need) newcap <<= 1;
    arr   = mem_resize(seq_t, arr, newcap);
    pref  = mem_resize(seq_t, pref, newcap);
    cap   = newcap;
}

seq_t a(idx_t n) {
    /* Ensure the cache can hold up to n */
    ensure_capacity((size_t)n + 1);

    /* Compute missing values up to n */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            arr[1]  = (seq_t)1;
            pref[1] = arr[1];
        } else if (i == 2) {
            arr[2]  = (seq_t)2;
            pref[2] = pref[1] + arr[2];
        } else if (i == 3) {
            arr[3]  = (seq_t)3;
            pref[3] = pref[2] + arr[3];
        } else {
            idx_t m = compute_m((idx_t)i);
            seq_t sumPrev = pref[i - 1];
            arr[i] = sumPrev + arr[m];
            pref[i] = sumPrev + arr[i];
        }
    }
    len = (size_t)n;
    return arr[n];
}
