/*
 * A115263  Correlation triangle for floor((n+2)/2).
 * https://oeis.org/A115263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115263: Flattened correlation triangle for floor((n+2)/2).
   The triangle T(r,k) is defined by
       T(r,k) = sum_{j=0}^{min(k,r-k)} floor((k-j+2)/2) * floor((r-k-j+2)/2)
   This function returns the n‑th entry of that triangle read row by row
   (row 0 has 1 entry, row 1 has 2 entries, etc.).
*/

seq_t a(idx_t n)
{
    /* --- Find the row r such that
       r*(r+1)/2 <= n < (r+1)*(r+2)/2  --- */
    /* Compute s = floor(sqrt(8*n+1)) safely using 128‑bit arithmetic. */
    unsigned __int128 z = (unsigned __int128)8 * n + 1;   /* up to 2^67 fits in 128 bits */
    uint64_t lo = 0, hi = (1ULL << 32); /* hi is > sqrt(8*2^64) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) / 2;
        if ((unsigned __int128)mid * mid <= z)
            lo = mid;
        else
            hi = mid - 1;
    }
    uint64_t s = lo;                    /* floor(sqrt(8*n+1)) */
    uint64_t r = (s - 1) / 2;           /* floor((sqrt(8*n+1)-1)/2) */

    /* --- Compute column k  --- */
    uint64_t base = (unsigned __int128)r * (r + 1) / 2; /* r*(r+1)/2 */
    uint64_t k = n - base;

    /* --- Compute T(r,k) --- */
    uint64_t m = k < (r - k) ? k : (r - k);
    seq_t total = 0;
    for (uint64_t j = 0; j <= m; ++j) {
        seq_t a_val = (seq_t)(k - j + 2) / 2;          /* floor((k-j+2)/2) */
        seq_t b_val = (seq_t)(r - k - j + 2) / 2;      /* floor((r-k-j+2)/2) */
        total += a_val * b_val;
    }
    return total;
}
