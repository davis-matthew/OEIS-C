/*
 * A229792  Numbers n with the property that there exists an m such that n is
 *          the sum of m distinct nonzero digits (with 1<m<10) in a unique
 *          way.
 * https://oeis.org/A229792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A229792: Numbers that are the sum of a unique set of m distinct digits (1 ≤ m ≤ 9, m > 1). 
   Computed by exhaustive enumeration of all 2⁹–1 digit subsets. 
   The resulting sums are sorted ascending. 
*/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MAX_DIGITS 9
#define MAX_SUM 45

static seq_t seq_unique[MAX_SUM + 1];
static size_t seq_unique_len = 0;
static bool seq_initialized = false;

/* simple popcount for 9‑bit masks */
static int popcnt(int x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

static void init_unique(void) {
    if (seq_initialized) return;
    int cnt[MAX_SUM + 1][MAX_DIGITS + 1] = {{0}};
    /* enumerate all non‑empty subsets of digits 1..9 */
    for (int mask = 1; mask < (1 << MAX_DIGITS); ++mask) {
        int m = popcnt(mask);
        if (m < 2) continue;            /* need at least two distinct digits */
        int sum = 0;
        /* compute sum of selected digits */
        for (int d = 0; d < MAX_DIGITS; ++d) {
            if (mask & (1 << d)) sum += d + 1;
        }
        if (sum > MAX_SUM) continue;      /* just in case */
        ++cnt[sum][m];
    }
    /* collect sums that have a unique representation for some m */
    for (int s = 0; s <= MAX_SUM; ++s) {
        bool unique_any = false;
        for (int m = 2; m <= MAX_DIGITS; ++m) {
            if (cnt[s][m] == 1) { unique_any = true; break; }
        }
        if (unique_any) {
            seq_unique[seq_unique_len++] = (seq_t)s;
        }
    }
    seq_initialized = true;
}

seq_t a(idx_t n) {
    init_unique();
    if (n == 0 || n > seq_unique_len) return (seq_t)0;  /* undefined for out‑of‑range */
    return seq_unique[n - 1];
}
