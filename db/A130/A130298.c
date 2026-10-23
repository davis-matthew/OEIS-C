/*
 * A130298  A051340 * A130296.
 * https://oeis.org/A130298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r = the unique integer such that r(r-1)/2 < n ≤ r(r+1)/2. *
 *            Let j = n - r(r-1)/2 (1 ≤ j ≤ r). * Then * a(n) = r*(3r-1)/2 if
 *            j = 1 (first element of the row) * a(n) = 2r - j otherwise
 *            (remaining elements decrease by 1)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Find r by binary search on the triangular numbers */
    idx_t low  = 1;
    idx_t high = n;                 /* triangular(high) ≥ high for any high ≥ 1 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2; /* triangular(mid) */

        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t r = low;

    /*  Compute j = n - previous triangular number */
    idx_t prev = (idx_t)(((__int128)(r - 1) * r) / 2); /* (r-1)r/2 */
    idx_t j = n - prev;                               /* 1 ≤ j ≤ r */

    if (j == 1) {
        /*  a(n) = r*(3r-1)/2  (first element of the r‑th row) */
        return (seq_t)r * ((seq_t)r * 3 - 1) / 2;
    } else {
        /*  a(n) = 2r - j  (remaining elements decrease by 1) */
        return (seq_t)2 * r - j;
    }
}
