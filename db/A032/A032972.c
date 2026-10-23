/*
 * A032972  Numbers with the property that all pairs of consecutive base-9
 *          digits differ by more than 1.
 * https://oeis.org/A032972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sequence of positive integers whose base‑9 representation
 *            has the property that the absolute difference between every pair
 *            of adjacent digits is strictly greater than 1
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define MAX_LEN 20                    /* enough for n up to about 10⁴ */

static bool initialized = false;
static idx_t g[9][MAX_LEN];          /* g[d][l] – number of sequences of length l
                                       following a last digit d */
static idx_t cum[MAX_LEN + 1];       /* cum[L] – total numbers with ≤ L digits */

static void init(void) {
    if (initialized) return;
    /* g[d][0] = 1 for all d */
    for (int d = 0; d < 9; ++d) g[d][0] = 1;
    /* compute g for lengths 1 .. MAX_LEN-1 */
    for (int l = 1; l < MAX_LEN; ++l) {
        for (int d = 0; d < 9; ++d) {
            idx_t sum = 0;
            for (int e = 0; e < 9; ++e) {
                int diff = d > e ? d - e : e - d;
                if (diff > 1) sum += g[e][l - 1];
            }
            g[d][l] = sum;
        }
    }
    /* cumulative counts */
    cum[0] = 0;
    for (int L = 1; L <= MAX_LEN; ++L) {
        idx_t total = 0;
        for (int d = 1; d < 9; ++d) {      /* first digit cannot be 0 */
            total += g[d][L - 1];
        }
        cum[L] = cum[L - 1] + total;
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined, but harmless */
    init();

    /* find length L such that cum[L-1] < n <= cum[L] */
    int L;
    for (L = 1; L <= MAX_LEN; ++L) {
        if (cum[L] >= n) break;
    }
    if (L > MAX_LEN) return (seq_t)0;  /* out of precomputed range */

    idx_t offset = n - cum[L - 1] - 1;  /* 0‑based index within length‑L numbers */

    seq_t result = 0;
    int prev = -1;
    int rem = L - 1;                     /* remaining digits after current */

    for (int pos = 0; pos < L; ++pos) {
        int chosen = 0;
        if (pos == 0) {
            /* first digit cannot be 0 */
            for (int d = 1; d < 9; ++d) {
                idx_t cnt = g[d][rem];
                if (offset < cnt) {
                    chosen = d;
                    break;
                } else {
                    offset -= cnt;
                }
            }
        } else {
            for (int d = 0; d < 9; ++d) {
                int diff = prev > d ? prev - d : d - prev;
                if (diff > 1) {
                    idx_t cnt = g[d][rem];
                    if (offset < cnt) {
                        chosen = d;
                        break;
                    } else {
                        offset -= cnt;
                    }
                }
            }
        }
        result = result * (seq_t)9 + (seq_t)chosen;
        prev = chosen;
        --rem;
    }
    return result;
}
