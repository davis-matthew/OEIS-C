/*
 * A190362  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=sqrt(5/4), t=sqrt(4/5).
 * https://oeis.org/A190362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*sqrt(4/5)⌋ + ⌊4*n/5⌋ (OEIS A190362)
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit type for safe intermediate arithmetic. */
    unsigned __int128 N   = n;               /* cast index to 128‑bit */
    unsigned __int128 Q   = (unsigned __int128)4 * N * N; /* 4*n*n   */
    unsigned __int128 div5 = Q / 5;          /* floor((4*n*n)/5) */

    /* Integer sqrt: find largest x with x*x <= div5.
       Use division to avoid overflow. */
    unsigned __int128 lo = 0;
    unsigned __int128 hi = (unsigned __int128)1 << 64;   /* hi > max sqrt(div5) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid <= div5 / mid)   /* mid*mid <= div5   */
            lo = mid;
        else
            hi = mid - 1;
    }
    seq_t part1 = (seq_t)lo;                 /* floor sqrt((4*n*n)/5) */

    /* floor(4*n/5) – compute in 128‑bit to avoid overflow. */
    unsigned __int128 temp   = (unsigned __int128)4 * N;
    seq_t part2 = (seq_t)(temp / 5);

    /* Assemble final result. */
    seq_t res = (seq_t)n + part1 + part2;
    return res;
}
