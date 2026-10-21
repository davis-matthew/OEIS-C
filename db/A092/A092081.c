/*
 * A092081  Triangle of certain double factorials.
 * https://oeis.org/A092081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A092081: Triangle of certain double factorials
 *
 * a(m, n) = (m + n)!! / (m - n)!! , 0 ≤ n ≤ m, otherwise 0
 * The 1‑D array is read by SW–NE diagonals:
 *   a(0), a(1,0), a(1,1), a(2,0), a(2,1), a(2,2), …
 *
 * a(n) is the typedef seq_t value at index n (idx_t n ≥ 0).
 *
 * The computation uses only integer arithmetic; no floating point or
 * <math.h>.  An integer square‑root routine for 128‑bit values is
 * implemented locally.
 */
static inline idx_t
isqrt_u128(unsigned __int128 x)
{
    /* floor(sqrt(x)) for x < 2^128 */
    unsigned __int128 low  = 0;
    unsigned __int128 high = (((unsigned __int128)1) << 64) - 1;   /* sqrt(2^128) < 2^64 */
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x)
            return (idx_t)mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (idx_t)high;
}

seq_t
a(idx_t n)
{
    /* Find the triangle row m such that m(m+1)/2 ≤ n < (m+1)(m+2)/2. */
    unsigned __int128 temp = ((unsigned __int128)1) + ((unsigned __int128)8) * n;
    idx_t t = isqrt_u128(temp);          /* floor(sqrt(1+8n)) */
    idx_t m = (t - 1) / 2;                /* row index */

    /* Offset (total terms before this row).  Use 128‑bit to avoid overflow. */
    unsigned __int128 ofs128 = ((unsigned __int128)m * (m + 1)) / 2;
    idx_t offset = (idx_t)ofs128;         /* guaranteed to fit in idx_t */

    idx_t col = n - offset;               /* column within the row (0 ≤ col ≤ m) */

    /* Compute the double‑factorial ratio as a product of numbers
       of the same parity from (m-col)+2 to (m+col), step 2. */
    idx_t start = (m - col) + 2;
    idx_t end   = m + col;

    seq_t result = 1;
    for (idx_t v = start; v <= end; v += 2) {
        result *= (seq_t)v;
    }
    return result;
}
