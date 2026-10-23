/*
 * A194287  Triangular array: g(n,k)=number of fractional parts (i*sqrt(2)) in
 *          interval [(k-1)/n, k/n], for 1<=i<=n^2, 1<=k<=n.
 * https://oeis.org/A194287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(row, col) * * For a given index n the corresponding row
 *            r and column c of the * triangular array are found by
 *            incrementing a running sum until it * reaches n. Then g(r,c)
 *            counts the numbers i∈[1,r²] for which the * fractional part of
 *            i·√2 lies in the interval [(c-1)/r , c/r]. The * condi...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t P = 665857ULL;  /* numerator of √2 approximation  */
static const uint64_t Q = 470832ULL;  /* denominator of √2 approximation  */

seq_t a(idx_t n)
{
    /* Find the triangular row r and column c for index n (1‑based). */
    idx_t r = 1;
    idx_t sum = 0;
    while (sum + r < n) {
        sum += r;
        ++r;
    }
    idx_t c = n - sum;          /* 1‑based column number   */

    /* Count i ∈ [1, r²] with floor(r·{i·√2}) = c–1. */
    idx_t limit = r * r;
    seq_t count = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        /* floor(i·√2) */
        unsigned __int128 mul = (unsigned __int128)i * P;
        idx_t floor_i = (idx_t)(mul / Q);

        /* floor(r·i·√2) */
        unsigned __int128 mul_big = (unsigned __int128)i * r * P;
        idx_t floor_big = (idx_t)(mul_big / Q);

        /* floor(r·{i·√2}) = floor(r·i·√2) – r·floor(i·√2) */
        idx_t t = floor_big - floor_i * r;

        if (t == c - 1U) {
            ++count;
        }
    }

    return (seq_t)count;
}
