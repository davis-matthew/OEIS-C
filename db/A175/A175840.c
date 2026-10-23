/*
 * A175840  Mirror image of Nicomachus' table: T(n,k) = 3^(n-k)*2^k for n>=0
 *          and 0<=k<=n.
 * https://oeis.org/A175840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 3^(r-k) * 2^k where r = floor((sqrt(8*n+1)-1)/2), the row
 *            index, k = n - r*(r+1)/2, the column index (0‑based in the row)
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 230, u64 n <= 860,
 *            u128 n <= 3320, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 <= n.  Use binary search
       to avoid overflow; intermediate products are done in 128‑bit. */
    idx_t low  = 0;
    idx_t high = n;
    idx_t r    = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;           /* mid = (low+high)/2 */
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) {
            r = mid;
            low = mid + 1;                       /* try a larger r */
        } else {
            if (mid == 0) break;
            high = mid - 1;                     /* try a smaller r */
        }
    }

    /* offset of the first term of row r */
    unsigned __int128 offset128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t offset = (idx_t)offset128;

    /* column index k in the row */
    idx_t k   = n - offset;
    idx_t e3  = r - k;     /* exponent of 3: (r-k) */
    idx_t e2  = k;         /* exponent of 2: k      */

    /* Multiply the required powers of 3 and 2.  Loops are short
       because r is at most about sqrt(2*n). */
    seq_t val = 1;
    for (idx_t i = 0; i < e3; ++i) val *= (seq_t)3;
    for (idx_t i = 0; i < e2; ++i) val *= (seq_t)2;

    return val;
}
