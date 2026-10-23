/*
 * A026432  a(n) = least positive integer > a(n-1) and not a(i)*a(j)-1 for
 *          1<=i<j<=n, with initial terms 1,3.
 * https://oeis.org/A026432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026432:
 * a(1) = 1, a(2) = 3.
 * a(n) = least positive integer > a(n-1) that is not
 *        expressible as a(i)*a(j) - 1 for any 1 ≤ i < j ≤ n. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *seq_data = NULL;         /* array of already computed terms */
static idx_t seq_len = 0;              /* number of terms stored */
static idx_t seq_cap = 0;              /* current capacity of seq_data */

/* binary search: returns true if 'value' is in seq_data[0..len-1] */
static bool seq_contains(idx_t len, seq_t value) {
    idx_t lo = 0, hi = (len > 0) ? len - 1 : 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        seq_t midv = seq_data[mid];
        if (midv == value) return true;
        if (midv < value) lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return false;
}

/* integer square‑root for a 64‑bit unsigned integer (no overflow) */
static uint64_t int_sqrt_u64(uint64_t x) {
    uint64_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid != 0 && mid <= x / mid) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* test whether candidate is forbidden by any previous pair */
static bool is_forbidden(seq_t cand, idx_t len) {
    uint64_t t = (uint64_t)(cand + 1);            /* candidate + 1 */
    uint64_t sq = int_sqrt_u64(t);                /* floor(sqrt(t)) */

    for (uint64_t d = 1; d <= sq; ++d) {
        if (t % d != 0) continue;
        uint64_t e = t / d;
        if (d == e) continue;                    /* need two distinct factors */
        if (seq_contains(len, (seq_t)d) && seq_contains(len, (seq_t)e))
            return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;                 /* undefined for offset 1 */

    /* initialise with first two terms if needed */
    if (seq_len < 2) {
        seq_cap = 64;
        seq_data = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_data[0] = (seq_t)1;
        seq_data[1] = (seq_t)3;
        seq_len = 2;
    }

    /* compute missing terms up to n (any order of calls is fine) */
    while (seq_len < n) {
        seq_t prev = seq_data[seq_len - 1];
        seq_t cand = prev + 1;
        while (true) {
            if (!is_forbidden(cand, seq_len)) {
                if (seq_len == seq_cap) {
                    seq_cap <<= 1;
                    seq_data = (seq_t *)realloc(seq_data,
                                                seq_cap * sizeof(seq_t));
                }
                seq_data[seq_len++] = cand;
                break;
            }
            ++cand;
        }
    }
    return seq_data[n - 1];
}
