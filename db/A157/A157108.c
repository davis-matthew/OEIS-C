/*
 * A157108  Triangle, read by rows, T(n, k) = binomial(n*binomial(n, k), k).
 * https://oeis.org/A157108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 71, u128 n <= 128,
 *            big any n
 * Helpers:   binom_seq_Nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_seq_Nk.h"
/* The AC-Leet sequence A157108:  T(n,k) = binomial(n*binomial(n,k),k),
 * flattened row‑by‑row (offset 0).  The function computes a(n),
 * where n is the flattened index. */

/* ------------------------------------------------------------------ */
/*  Minimal helpers for binomial coefficients in seq_t arithmetic      */
static seq_t binom_small(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        result = result * (seq_t)(n - k + i);
        result = result / (seq_t)i;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/*  Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2, 0‑based. */
static idx_t find_row(idx_t n) {
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n) high <<= 1;   /* grow limit      */
    while (low + 1 < high) {                          /* binary search   */
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
seq_t a(idx_t n) {
    /* Determine row r and column c from flattened index n. */
    idx_t r = find_row(n);
    idx_t before = (r * (r + 1)) / 2;
    idx_t c = n - before;          /* 0 <= c <= r */

    /* binomial(r, c)  (small arguments, fit in idx_t). */
    seq_t binom_rc = binom_small(r, c);

    /* N = r * binom_rc  (N may be large, use seq_t arithmetic). */
    seq_t N = (seq_t)r * binom_rc;

    /* T(r,c) = binomial(N, c). */
    return binom_seq_Nk(N, c);
}
