/*
 * A068527  Difference between smallest square >= n and n.
 * https://oeis.org/A068527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(sqrt(n))^2 - n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the difference between the smallest square ≥ n and n. */

static idx_t floor_sqrt_idx(idx_t n)
{
    /* Fast integer square‑root (floor) for 0 ≤ n < 2^64. */
    if (n < 2) return n;          /* sqrt(0)=0, sqrt(1)=1          */
    idx_t low  = 0;
    idx_t high = (n >> 1) + 1;    /* sqrt(n) ≤ n/2 for n≥2, safe  */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;   /* exact match                 */
        if (sq < n) low = mid + 1;
        else        high = mid - 1;
    }
    return high; /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    idx_t r   = floor_sqrt_idx(n);          /* floor √n          */
    seq_t sq  = (seq_t)r * (seq_t)r;        /* r²                */
    if (sq == (seq_t)n)                    /* already a square  */
        return 0;
    seq_t nxt = (seq_t)(r + 1);             /* ceil √n           */
    nxt = nxt * nxt;                       /* (r+1)²            */
    return nxt - (seq_t)n;                 /* difference        */
}
