/*
 * A209279  First inverse function (numbers of rows) for pairing function
 *          A185180.
 * https://oeis.org/A209279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((t+2)/2) + floor(i/2)*(-1)^(i+t+1) where t = max {
 *            t | t*(t+1)/2 < n }, i = n - t*(t+1)/2
 * Range:     u8 n <= 9870, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a binary search on t to avoid floating‑point operations. */

seq_t a(idx_t n)
{
    /* Find the largest t such that t*(t+1)/2 < n */
    __int128 l = 0;
    __int128 r = n;          /* r is at most n, fits in idx_t */
    while (l < r) {
        __int128 m = (l + r + 1) >> 1;           /* upper mid to avoid missing t */
        __int128 val = (m * (m + 1)) >> 1;      /* m*(m+1)/2 in 128‑bit */
        if (val < (__int128)n)
            l = m;
        else
            r = m - 1;
    }
    idx_t t = (idx_t)l;
    idx_t tri = (idx_t)(((__int128)t * (t + 1)) >> 1);
    idx_t i = n - tri;          /* 1 ≤ i ≤ t+1 */

    seq_t s1 = (seq_t)((t + 2) / 2);     /* floor((t+2)/2) */
    seq_t s2 = (seq_t)(i / 2);           /* floor(i/2) */
    int parity = (int)((i + t + 1) & 1); /* 0 = even => +1, 1 = odd => -1 */

    seq_t res = s1;
    if (parity == 0)
        res += s2;        /* sign +1 */
    else
        res -= s2;        /* sign -1 */

    return res;
}
