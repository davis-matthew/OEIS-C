/*
 * A051340  A simple 2-dimensional array, read by antidiagonals: T[i,j] = 1
 *          for j>0, T[i,0] = i+1; i,j = 0,1,2,3,...
 * https://oeis.org/A051340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   a(n) = 1 for all n, except when n = k*(k+3)/2 for k ≥ 0; then
 *            a(n) = k+1
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute D = 9 + 8*n in 128‑bit arithmetic */
    unsigned __int128 D = (unsigned __int128)9 + (unsigned __int128)8 * n;

    /* binary search for integer sqrt of D; sqrt(D) ≤ 2^34 < 2^35 */
    uint64_t low  = 0;
    uint64_t high = (uint64_t)1 << 35;            /* 2^35 > sqrt(max D) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * (unsigned __int128)mid;
        if (mid2 <= D)
            low = mid;
        else
            high = mid - 1;
    }
    uint64_t v = low;                 /* floor(sqrt(D)) */

    /* check perfect square and divisibility condition */
    if ((unsigned __int128)v * v != D)          /* not a perfect square */
        return (seq_t)1;
    if (v < 3)                                  /* v = 3 corresponds to k = 0 */
        return (seq_t)1;
    if (((v - 3) & 1) != 0)                     /* (v-3) must be even */
        return (seq_t)1;

    uint64_t k = (v - 3) >> 1;                  /* (v-3)/2 */
    return (seq_t)(k + 1);
}
