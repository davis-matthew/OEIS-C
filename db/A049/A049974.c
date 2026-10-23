/*
 * A049974  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = n -
 *          1 - 2^p and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1 and a(2) = a(3) = 3.
 * https://oeis.org/A049974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=3; for n ≥ 4: let v = n‑1, p = ⌊log₂(v‑1)⌋
 *            (the unique integer with 2ᵖ < v ≤ 2ᵖ⁺¹), m = v – 2ᵖ; then a(n) =
 *            (a(1)+…+a(v)) + a(m)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

static seq_t vals[10001];   /* vals[i] == a(i), 1‑based */
static seq_t sums[10001];   /* sums[i] == a(1)+…+a(i) */
static size_t computed = 0; /* largest i that has been initialized */

/* Main sequence function – external linkage */
seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for non‑positive n */

    /* Ensure arrays big enough for the requested n */
    if (n >= sizeof(vals)/sizeof(vals[0])) {
        /* If we need more than the pre‑allocated size, compute on the fly
           without memoisation (rare for n≈10⁴). */
        seq_t sum = 0;
        for (idx_t i = 1; i <= n; ++i) {
            idx_t v = i - 1;
            if (i >= 4 && v >= 1) {
                idx_t p = floor_log2_uint64(v - 1);
                idx_t pow2 = ((idx_t)1) << p;
                idx_t m = v - pow2;
                sum = sum + a(i - 1) + a(m);
            } else if (i == 1) {
                sum = 1;
            } else if (i == 2 || i == 3) {
                sum = 3;
            }
        }
        return sum;
    }

    /* Initialise the first three terms once */
    if (computed < 3) {
        vals[1] = (seq_t)1;
        sums[1] = vals[1];
        vals[2] = (seq_t)3;
        sums[2] = sums[1] + vals[2];
        vals[3] = (seq_t)3;
        sums[3] = sums[2] + vals[3];
        computed = 3;
    }

    if (n <= computed) return vals[n];

    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t v = i - 1;
        idx_t p = floor_log2_uint64(v - 1);
        idx_t pow2 = ((idx_t)1) << p;
        idx_t m = v - pow2;
        seq_t term = sums[v] + vals[m];
        vals[i] = term;
        sums[i] = sums[i - 1] + term;
    }
    computed = n;
    return vals[n];
}
