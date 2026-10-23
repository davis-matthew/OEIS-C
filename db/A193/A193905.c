/*
 * A193905  Mirror of the triangle A193904.
 * https://oeis.org/A193905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = w(n, n-k) where w(n,k) = 2^(n+k) – 2^max(2k‑1,0)
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle row r starts at linear index offset_row(r)=r*(r+1)/2. */

#include "../utils/offset_row.h"

/* Compute 2^e in the seq_t type – works for built‑in integer or
   arbitrary‑precision seq_t. */
static inline seq_t pow2_seq(idx_t e) {
    seq_t v = 1;
    for (idx_t i = 0; i < e; ++i) v *= 2;
    return v;
}

seq_t a(idx_t n) {
    /* a(0) = 1 */
    if (n == 0) return 1;

    /* Find the row r such that offset_row(r) ≤ n < offset_row(r+1) */
    idx_t low = 0, high = 1;
    while (offset_row(high) <= n) high <<= 1;   /* exponential search */

    while (low + 1 < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (offset_row(mid) <= n) low = mid;
        else                     high = mid;
    }
    idx_t r = low;                     /* r is the row containing n */
    idx_t k = n - offset_row(r);       /* position within the row (0 … r) */

    idx_t exp1 = r + k;
    idx_t exp2 = (k == 0) ? 0 : (2 * k - 1);

    seq_t val = pow2_seq(exp1);
    seq_t sub = pow2_seq(exp2);
    val -= sub;
    return val;
}
