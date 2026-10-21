/*
 * A203951  Symmetric matrix based on (1,0,0,0,1,0,0,0,...), by antidiagonals.
 * https://oeis.org/A203951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* a(n) = the entry M[i,j] of the symmetric matrix M = T′·T
   enumerated along anti‑diagonals.
   For n ≥ 1, let k be the smallest integer with k*(k+1)/2 ≥ n.
   Let t = n - k*(k-1)/2 - 1, i = t+1, j = k-t.
   If i mod 4 ≠ j mod 4, a(n)=0.
   Otherwise a(n) = ⌊(min(i,j)-first)/4⌋ + 1,
   where first = ((i-1) mod 4)+1.  */

static idx_t diag_of(idx_t n)
/* return minimal k with k*(k+1)/2 ≥ n, for n ≥ 1 */
{
    idx_t low = 0;
    idx_t high = n;              /* k never exceeds n   */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    return high;                  /* high ≥ 1            */
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of bounds guard */

    idx_t k = diag_of(n);                /* diagonal index (≥1) */

    /* cumulative elements before this diagonal */
    idx_t prev = (idx_t)((unsigned __int128)k * (k - 1) / 2);

    idx_t t   = n - prev - 1; /* 0‑based position within the diagonal */
    idx_t i   = t + 1;        /* row (1‑based) */
    idx_t j   = k - t;        /* column (1‑based) */

    /* M[i,j] is non‑zero only if i ≡ j (mod 4) */
    if ((i & 3) != (j & 3)) return (seq_t)0;

    /* smallest positive t ≡ i (mod 4) */
    idx_t first_t = ((i - 1) & 3) + 1;

    idx_t m = (i < j) ? i : j;          /* min(i,j) */
    if (m < first_t) return (seq_t)0;   /* no valid t in range */

    /* number of t in [1..m] with that residue */
    idx_t count = (m - first_t) >> 2;   /* divide by 4 */
    count += 1;

    return (seq_t)count;
}
