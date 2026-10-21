/*
 * A066324  Number of endofunctions on n labeled points constructed from k
 *          rooted trees.
 * https://oeis.org/A066324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) * where r is the row number with r(r‑1)/2 < n ≤
 *            r(r+1)/2, * c = n – r(r‑1)/2, * and T(r,c) = c · r^(r‑c) ·
 *            (r‑1)! / (r‑c)! (1 ≤ c ≤ r)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It enumerates the flattened triangle of T(n,k). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* for safety, although n ≥ 1 by definition */

    /* Find r such that r*(r-1)/2 < n ≤ r*(r+1)/2 */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* Column index within the row */
    idx_t prev = (r - 1) * r / 2;   /* number of entries before row r */
    idx_t c = n - prev;             /* 1 ≤ c ≤ r */

    /* Compute r^(r‑c) */
    seq_t pow_val = 1;
    idx_t exp = r - c;
    seq_t base = (seq_t)r;
    while (exp) {
        if (exp & 1ULL) pow_val *= base;
        exp >>= 1;
        if (exp) base *= base;
    }

    /* Compute falling factorial (r‑1)_(c‑1) = (r‑1)! / (r‑c)! */
    seq_t falling = 1;
    for (idx_t i = 1; i < c; ++i) {
        falling *= (seq_t)(r - i);
    }

    /* Final result: c · pow_val · falling */
    seq_t result = (seq_t)c * pow_val * falling;
    return result;
}
