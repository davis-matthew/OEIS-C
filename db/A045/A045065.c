/*
 * A045065  Numbers whose base-4 representation contains no 0's and exactly
 *          four 3's.
 * https://oeis.org/A045065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base-4 representation
 *            contains no 0s and exactly four 3s
 * Range:     u8 n <= 1, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   comb_capped, mul_capped, pow2_capped
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/comb_capped.h"
#include "../utils/mul_capped.h"
#include "../utils/pow2_capped.h"

/* Main sequence generator. */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n < 1 */

    /* Determine the number of digits d for the n-th term. */
    idx_t remaining = n;
    idx_t d = 4;
    while (1) {
        idx_t cap = remaining;
        idx_t count_d = mul_capped(
            comb_capped(d, 4, cap),
            pow2_capped(d - 4, cap),
            cap
        );
        if (count_d == 0 || count_d >= remaining) {
            break;
        }
        remaining -= count_d;
        ++d;
    }

    /* remaining is now the index (1‑based) within the numbers of length d. */
    idx_t k = remaining;
    idx_t t = 4;                     /* remaining 3’s to place */
    seq_t value = 0;

    for (idx_t pos = 0; pos < d; ++pos) {
        idx_t r = d - pos;          /* positions still free */

        /* Try digit 1. */
        idx_t cnt;
        if (t > r - 1)
            cnt = 0;
        else {
            idx_t cap = k - 1;
            cnt = mul_capped(
                comb_capped(r - 1, t, cap),
                pow2_capped(r - 1 - t, cap),
                cap
            );
        }
        if (k <= cnt) {
            value = value * 4 + (seq_t)1;
            continue;
        }
        k -= cnt;

        /* Try digit 2 (same count as 1). */
        if (t > r - 1)
            cnt = 0;
        else {
            idx_t cap = k - 1;
            cnt = mul_capped(
                comb_capped(r - 1, t, cap),
                pow2_capped(r - 1 - t, cap),
                cap
            );
        }
        if (k <= cnt) {
            value = value * 4 + (seq_t)2;
            continue;
        }
        k -= cnt;

        /* Remaining digit must be 3. */
        value = value * 4 + (seq_t)3;
        --t;
    }

    return value;
}
