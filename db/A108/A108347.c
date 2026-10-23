/*
 * A108347  Numbers of the form (3^i)*(5^j)*(7^k), with i, j, k >= 0.
 * https://oeis.org/A108347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numbers of the form 3^i·5^j·7^k (i,j,k ≥ 0) sorted
 *            increasingly. The implementation uses the classic smooth‑number
 *            generator for primes 3,5,7
 * Range:     u8 n <= 23, u16 n <= 115, u32 n <= 711, u64 n <= 4927,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_seq_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_seq_cap.h"

static seq_t *smooth = NULL;           /* array of generated terms                */
static size_t smooth_cap = 0;          /* allocated capacity                       */
static idx_t smooth_len = 0;           /* number of terms already filled           */
static idx_t idx3 = 0, idx5 = 0, idx7 = 0;   /* pointers for next multiples of 3,5,7 */

static inline void compute_to(idx_t n)
{
    if (smooth_len == 0) {
        ensure_seq_cap(&smooth, &smooth_cap, 1);
        smooth[0] = (seq_t)1;
        smooth_len = 1;
        idx3 = idx5 = idx7 = 0;
    }

    if (smooth_len >= n) return;

    ensure_seq_cap(&smooth, &smooth_cap, (size_t)n);
    for (; smooth_len < n; ++smooth_len) {
        seq_t c3 = smooth[idx3] * (seq_t)3;
        seq_t c5 = smooth[idx5] * (seq_t)5;
        seq_t c7 = smooth[idx7] * (seq_t)7;

        seq_t next = c3;
        if (c5 < next) next = c5;
        if (c7 < next) next = c7;

        smooth[smooth_len] = next;

        if (next == c3) ++idx3;
        if (next == c5) ++idx5;
        if (next == c7) ++idx7;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for n==0, but safe return      */
    compute_to(n);
    return smooth[n - 1];
}
