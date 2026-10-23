/*
 * A173739  Square array read by antidiagonals (n >= 1, k >= 2): T(n,k) =
 *          b(n,k) + b(k-1,n+1), where b(n,k) = ((1 + sqrt(k))^n - (1 -
 *          sqrt(k))^n)/(2*sqrt(k)).
 * https://oeis.org/A173739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173739: Square array read by antidiagonals.
   T(n,k) = b(n,k) + b(k-1,n+1) ,
   b(n,k) = ((1+√k)^n – (1-√k)^n)/(2√k)   (n≥1, k≥2)
   a(1) is the first term in the table. */
#include <stdint.h>

static seq_t compute_b(idx_t n, seq_t k)
{
    if (n == 0) return 0;
    seq_t k_minus1 = k - 1;    /* k≥2 guarantees non‑negative */
    seq_t prevprev = 0;        /* b(0,k) = 0 */
    seq_t prev     = 1;        /* b(1,k) = 1 */

    if (n == 1) return prev;

    seq_t curr;
    for (idx_t m = 2; m <= n; ++m) {
        curr = 2 * prev + k_minus1 * prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}

static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t part1 = compute_b(n, (seq_t)k);
    seq_t part2 = compute_b(k - 1, (seq_t)(n + 1));
    return part1 + part2;
}

seq_t a(idx_t n)
{
    /* Map the 1‑based index n into (row, col) of the array. */
    idx_t m   = n;
    idx_t cum = 0;
    idx_t d   = 3;  /* diagonal index (n + k)  */
    for (;;) {
        cum += d - 2;           /* elements in this diagonal */
        if (cum >= m) break;
        ++d;
    }

    idx_t prev_cum = cum - (d - 2);   /* total before this diagonal */
    idx_t offset   = m - prev_cum - 1;/* zero‑based offset within diagonal */
    idx_t row      = 1 + offset;      /* n coordinate */
    idx_t col      = d - row;         /* k coordinate */

    return compute_T(row, col);
}
