/*
 * A223451  Number of idempotent 2X2 -n..n matrices of rank 1.
 * https://oeis.org/A223451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of idempotent 2x2 integer matrices of rank 1 with entries
 *            in [‑n, n] The formula uses a characterization via the entries
 *            (a,b,c,d) satisfying M² = M. For a in [1‑n, 1+n] we let t = a ·
 *            (1‑a). For each such a the number of admissible (b,c) is: if t =
 *            0 → 4 n + 1 else → 2 × |{d ∈ ℕ : d ≤ n an...
 * Range:     u8 n <= 13, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; value for n=0 is unused */
    seq_t ans = 0;

    /* iterate a from 1‑n to 1+n (inclusive) */
    int64_t a_min = 1 - (int64_t)n;
    int64_t a_max = 1 + (int64_t)n;

    for (int64_t a_val = a_min; a_val <= a_max; ++a_val) {
        /* t = a*(1‑a) may be negative */
        int64_t t_val = a_val * (1 - a_val);   /* exact 64‑bit signed */
        seq_t abs_t = (t_val >= 0) ? (seq_t)t_val : -(seq_t)t_val;

        if (abs_t == 0) {                     /* a == 0 or a == 1 */
            /* All (b,c) with b‑c in [‑n,n] and bc = 0.  Count = 4n + 1. */
            ans += (seq_t)4 * n + (seq_t)1;
            continue;
        }

        /* Count positive divisors d of abs_t with d ≤ n and abs_t/d ≤ n. */
        idx_t limit = (idx_t)abs_t;
        if (limit > n) limit = n;            /* d > abs_t can never divide it */

        idx_t countDiv = 0;
        for (idx_t d = 1; d <= limit; ++d) {
            if ((abs_t % d) == 0) {
                seq_t q = abs_t / d;
                if (q <= n) ++countDiv;
            }
        }

        /* Each such divisor gives two ordered pairs (b,c) up to sign. */
        ans += (seq_t)countDiv * (seq_t)2;
    }

    return ans;
}
