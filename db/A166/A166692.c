/*
 * A166692  Triangle T(n,k) read by rows: T(n,k) = 2^(k-1), k>0, T(n,0) =
 *          (n+1) mod 2.
 * https://oeis.org/A166692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,c) where r is the row so that r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and c = n - r(r+1)/2. Then if c = 0 : a(n) = (r+1)
 *            mod 2 else : a(n) = 2^(c-1)
 * Range:     u8 n <= 53, u16 n <= 169, u32 n <= 593, u64 n <= 2209,
 *            u128 n <= 5049, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic, no floating point and no
   external libraries except the supplied pow2 helper. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find r by binary search on 0 ≤ r ≤ 2^32 (enough for all 64‑bit n). */
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 32;  /* 4,294,967,296 */
    idx_t r = 0;

    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* Compute mid*(mid+1)/2 carefully using 128‑bit to avoid overflow. */
        unsigned __int128 middle = (unsigned __int128)mid * (mid + 1) / 2;
        if (middle <= n) {
            r = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    /* Start index of row r.  The product fits in 64 bits, so 128‑bit helps. */
    idx_t start = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);

    /* Column within the row. */
    idx_t c = n - start;

    if (c == 0) {
        /* T(r,0) = (r+1) mod 2.  Cast the small integer result to seq_t. */
        return (seq_t)((r + 1) & 1ULL);
    } else {
        /* T(r,c) = 2^(c-1). */
        return pow2(c - 1);
    }
}
