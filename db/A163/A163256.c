/*
 * A163256  Fractal sequence of the interspersion A163253.
 * https://oeis.org/A163256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2500, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm:
   The sequence consists of blocks.  Block k (k≥1) has length 2k+1
   and contains the numbers

           1, 2, 4, 6, …, 2k, 3, 5, …, 2k+1

   The cumulative length up to block k is
           S(k) = k² + 2k
   (since it is the sum of 3,5,…,(2k+1)).

   To compute a(n):
     1. Find the smallest k with S(k) ≥ n (k ≈ √(n)).
     2. Let prev = S(k‑1) = (k‑1)² + 2(k‑1).
        pos = n – prev  (1 ≤ pos ≤ 2k+1).
     3. If pos==1 → 1
        else if pos==2 → 2
        else if pos ≤ k+1 → 2·(pos‑1)
        else → 2·(pos‑(k+1))+1
*/

static idx_t find_block(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val =
            ((unsigned __int128)mid * mid) +
            (unsigned __int128)2 * mid;
        if (val >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* 0 is outside the sequence, defined as 0 */

    idx_t k = find_block(n);
    idx_t km1 = k - 1;

    unsigned __int128 prev =
        ((unsigned __int128)km1 * km1) +
        (unsigned __int128)2 * km1;
    idx_t prevS = (idx_t)prev;

    idx_t pos = n - prevS;   /* 1‑based position inside the block */

    if (pos == 1)
        return 1;
    if (pos == 2)
        return 2;
    if (pos <= k + 1) {
        /* even numbers 4,6,…,2k  (pos‑1 even index) */
        return (seq_t)((seq_t)(pos - 1) * 2);
    }
    /* odd numbers 3,5,…,2k+1 */
    return (seq_t)((seq_t)(pos - (k + 1)) * 2 + 1);
}
