/*
 * A099020  Euler-Seidel matrix T(k,n) with start sequence A001147, read by
 *          antidiagonals.
 * https://oeis.org/A099020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   compute a(n) as the antidiagonal entry of the Euler–Seidel
 *            matrix generated from the start sequence A001147 (0 for odd
 *            indices, (2k‑1)!! for even indices). For a given global index n:
 *            - find the diagonal d with d(d+1)/2 ≤ n < (d+1)(d+2)/2 - offset
 *            = n – d(d+1)/2 - row r = d – offset - column j =...
 * Range:     u8 n <= 35, u16 n <= 77, u32 n <= 189, u64 n <= 527,
 *            u128 n <= 1539, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  // for bool, if the build uses it

static bool initialized = false;
static seq_t odd_fact[201];   // odd double‑factorials for k = 0 … 200

static void init(void) {
    if (initialized) return;
    odd_fact[0] = (seq_t)1;
    for (int k = 1; k <= 200; ++k) {
        odd_fact[k] = odd_fact[k - 1] * ((seq_t)(2 * k - 1));
    }
    initialized = true;
}

seq_t a(idx_t n) {
    init();

    /* find diagonal d: smallest d such that d(d+1)/2 > n, then d-1 */
    idx_t low  = 0;
    idx_t high = 1000;                     /* plenty large for n ≤ 10000 */
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if (mid * (mid + 1) / 2 <= n) low = mid;
        else                           high = mid - 1;
    }
    idx_t d = low;
    idx_t len_before = d * (d + 1) / 2;
    idx_t offset = n - len_before;          /* column index j */
    idx_t r = d - offset;                   /* row index */

    seq_t sum = 0;
    seq_t comb = 1;                          /* C(r,0) */
    for (idx_t t = 0; t <= r; ++t) {
        idx_t m = offset + t;                /* target column in row 0 */
        seq_t val = 0;
        if ((m & 1) == 0) {                 /* even index */
            idx_t k = m >> 1;                /* k = m/2 */
            val = odd_fact[k];
        }
        sum += comb * val;

        if (t < r) {                         /* update binomial coefficient */
            comb = comb * ((seq_t)(r - t)) / ((seq_t)(t + 1));
        }
    }
    return sum;
}
