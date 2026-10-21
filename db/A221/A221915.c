/*
 * A221915  Array for a certain labeled Morse code, recorded in standard
 *          order.
 * https://oeis.org/A221915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = product of dot positions of the Morse code that occupies
 *            index n in the flattened array of all labeled Morse codes
 *            (sorted by increasing dash count and lexicographic order of dash
 *            start positions)
 * Range:     u8 n <= 19, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Compute binomial coefficient C(n,k) using uint64 arithmetic. */
static inline idx_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Return the lexicographically rank-th combination of size m from {1..N}. */
static void get_combination(idx_t N, idx_t m, idx_t rank, idx_t *out)
{
    idx_t prev = 0;
    idx_t remaining = rank;
    for (idx_t pos = 1; pos <= m; ++pos) {
        for (idx_t x = prev + 1; x <= N - (m - pos); ++x) {
            idx_t cnt = binom_idx(N - x, m - pos);
            if (remaining < cnt) {
                out[pos - 1] = x;
                prev = x;
                break;
            } else {
                remaining -= cnt;
            }
        }
    }
}

seq_t a(idx_t n)
{
    /* Find the row r (0‑based) and position k within that row */
    idx_t fib_prev = 0;
    idx_t fib_curr = 1;        /* F(1) = 1  -> length of row r=0   */
    idx_t r = 0;
    idx_t row_len = fib_curr;  /* F(r+1) for current r       */
    idx_t cum = 0;             /* sum of lengths of rows < r  */

    while (n >= cum + row_len) {
        cum += row_len;
        ++r;
        idx_t fib_next = fib_prev + fib_curr;
        fib_prev = fib_curr;
        fib_curr = fib_next;
        row_len = fib_curr;    /* next row length = F(r+1)  */
    }
    idx_t k = n - cum;         /* 0‑based index inside row r */

    /* Determine number of dashes m in this code */
    idx_t m = 0;
    idx_t rem = k;
    while (1) {
        idx_t cnt = binom_idx(r - m, m);
        if (rem < cnt)
            break;
        rem -= cnt;
        ++m;
    }

    /* Get the specific combination of start positions for the m dashes */
    idx_t N = r - m;                 /* number of available indices for the combination  */
    idx_t comb[64] = {0};            /* r <= ~94 for 64‑bit indices; 64 is safe */
    if (m > 0)
        get_combination(N, m, rem, comb);

    /* Mark positions occupied by dashes (each dash covers two consecutive positions) */
    bool occ[128] = {0};             /* r <= 94, so this is safe */
    for (idx_t i = 0; i < m; ++i) {
        idx_t start = comb[i] + i;   /* convert combination to actual dash start position */
        occ[start] = true;
        occ[start + 1] = true;
    }

    /* Compute product of the remaining (dot) positions */
    seq_t prod = 1;
    for (idx_t j = 1; j <= r; ++j) {
        if (!occ[j])
            prod *= (seq_t)j;
    }
    return prod;
}
