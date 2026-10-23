/*
 * A158753  Triangle T(n, k) = A000032(2*(n-k) + 1), read by rows.
 * https://oeis.org/A158753
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 79, u32 n <= 277, u64 n <= 1082,
 *            u128 n <= 1226, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A158753: Triangle T(n,k) = A000032(2*(n‑k)+1), read by rows.
   With offset 2, the n-th term a(n) (n ≥ 2) is the (n‑2)-th entry of the flattened triangle.
   The first column follows f(1)=1, f(2)=4, f(n)=3*f(n‑1)-f(n‑2).
   In general T(r,c)=f(r‑c+1). */

#include <stdint.h>

/* Return the n-th term for n >= 2.  For n < 2 the value is undefined; 0 is returned. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return (seq_t)0;                     /* undefined below the offset */
    }

    /* Determine the row r and column c for the flattened index n */
    uint64_t m = n - 2;                   /* 0‑based offset in the triangle */
    uint64_t r = 0;                       /* row number (1‑based) */
    uint64_t sum = 0;                     /* cumulative count up to current row */

    /* Find smallest r with sum > m;  sum += r works safely because r <= 2^32 for all n < 2^64. */
    while (sum <= m) {
        r++;
        sum += r;
    }

    /* sum now equals r(r+1)/2  (> m);  previous cumulative count = sum - r */
    uint64_t prev = sum - r;              /* total terms before row r */
    uint64_t c = (m - prev) + 1;          /* column number (1‑based) */
    uint64_t k = r - c + 1;               /* index into the first‑column sequence f(k) */

    /* Compute the k-th term of the first column: f(1)=1, f(2)=4, f(n)=3*f(n‑1)-f(n‑2) */
    if (k == 1) {
        return (seq_t)1;
    }
    if (k == 2) {
        return (seq_t)4;
    }

    seq_t prev2 = (seq_t)1;              /* f(n‑2) */
    seq_t prev1 = (seq_t)4;              /* f(n‑1) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 3; i <= k; ++i) {
        cur = (seq_t)3 * prev1 - prev2;   /* f(i) = 3*f(i‑1) – f(i‑2) */
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
