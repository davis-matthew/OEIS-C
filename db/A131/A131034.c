/*
 * A131034  Triangle read by rows: A129686 * A051340 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A131034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle read by rows – a(n) = 1, 2, 1, 4, 1, 1, 6, 2, 1, 1, 8,
 *            2, 2, 1, 1, 10, … The triangle is composed of rows r (starting
 *            with r=0) of length r+1. Row r: a_{r,0}=2*r (with a_{0,0}=1),
 *            a_{r,k}=2 for 1≤k≤r-2, and a_{r,k}=1 for k=r-1 or k=r
 * Range:     u8 n <= 344, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
   Using integer arithmetic only. */
static uint64_t row_of_idx(idx_t n) {
    uint64_t low = 0, high = 1;
    /* Expand high until n < high*(high+1)/2 */
    while (( (__int128)high * (high + 1) / 2) <= ( (__int128)n)) {
        low = high;
        high <<= 1;            /* double high */
    }
    /* Binary search for the largest r with r*(r+1)/2 ≤ n */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (( (__int128)mid * (mid + 1) / 2) <= ( (__int128)n))
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute a(n) as described above */
seq_t a(idx_t n) {
    uint64_t r = row_of_idx(n);
    seq_t offset = (seq_t)r * ((seq_t)r + 1) / 2;   /* r*(r+1)/2 */
    uint64_t k = (uint64_t)n - (uint64_t)offset;    /* column index in row r */

    if (r == 0)
        return (seq_t)1;

    if (k == 0)
        return (seq_t)(2 * (seq_t)r);

    if (r >= 2 && k <= (uint64_t)(r - 2))
        return (seq_t)2;

    return (seq_t)1;
}
