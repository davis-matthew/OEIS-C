/*
 * A135387  Triangle read by rows, with (2, 1, 0, 0, 0, ...) in every column.
 * https://oeis.org/A135387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the unique integer such that r(r+1)/2 >= n > (r-1)r/2.
 *            Let j = n - (r-1)r/2. Then a(n) = 2 for n==1; otherwise a(n) = 0
 *            if j <= r-2, 1 if j == r-1, 2 if j == r
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* out of range (offset = 1) */
    if (n == 1) return (seq_t)2;

    /* find r such that r(r+1)/2 >= n */
    idx_t low = 1, high = 2;
    while ( ((seq_t)high * (high + 1) / 2) < n )
        high <<= 1;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ( ((seq_t)mid * (mid + 1) / 2) >= n )
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* compute j = n - (r-1)r/2 */
    seq_t tri_r_minus_1 = ((seq_t)(r - 1) * r) / 2;
    seq_t j = (seq_t)n - tri_r_minus_1;

    if (j <= (seq_t)(r - 2))
        return 0;
    else if (j == (seq_t)(r - 1))
        return 1;
    else
        return 2;
}
