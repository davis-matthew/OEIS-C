/*
 * A211161  Table T(n,k) = n, if k is odd, k/2 if k is even; n, k > 0, read by
 *          antidiagonals.
 * https://oeis.org/A211161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A211161 – Table T(n,k) = n if k odd, k/2 if k even, read by antidiagonals.
 *  The nth term a(n) is obtained by locating the (n)th entry in the
 *  antidiagonal order.  The implementation uses only integer arithmetic
 *  (no floating point, no external libraries except the basic types).
 *  It runs in O(log n) time and uses O(1) extra memory.
 */

#include <stdint.h>

/* Integer square‑root of a 128‑bit unsigned value.
 * Returns ⌊√v⌋.  This routine is used to compute the diagonal index
 * while staying within 128‑bit arithmetic. */
static idx_t isqrt128(unsigned __int128 v)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;   /* 2^64‑1 */
    idx_t result = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= v) {
            result = (idx_t)mid;
            low    = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return result;
}

/* Sequence entry for A211161. */
seq_t a(idx_t n)
{
    /* Compute 8*n – 7 as a 128‑bit value to avoid overflow. */
    unsigned __int128 v = (unsigned __int128)n * 8 - 7;

    /* Root of the quadratic used to locate the diagonal:
     *   t(t+1)/2 >= n   where t = ⌊(√(8n‑7) – 1)/2⌋. */
    idx_t s = isqrt128(v);
    idx_t t = (s - 1) / 2;

    /* Number of terms before this diagonal. */
    unsigned __int128 prev128 = (unsigned __int128)t * (t + 1) / 2;
    idx_t prev = (idx_t)prev128;

    /* Position inside the diagonal (1‑based). */
    idx_t offset = n - prev;          /* 1 ≤ offset ≤ t+1 */
    idx_t d      = t + 2;             /* diagonal index (n + k) */

    /* Row and column for this entry. */
    idx_t row = offset;               /* n‑coordinate */
    idx_t col = d - offset;           /* k‑coordinate */

    /* Evaluate T(n,k). */
    if ((col & 1) == 1) {          /* k odd → value = n */
        return (seq_t)row;
    } else {                       /* k even → value = k/2 */
        return (seq_t)(col / 2);
    }
}
