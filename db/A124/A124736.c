/*
 * A124736  Table where row n has k C(n,k-1) times.
 * https://oeis.org/A124736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Table where row r has integers 1..r+1 each repeated C(r,k-1) times.
   a(n) is the n‑th element of the flattened table. */
seq_t a(idx_t n)
{
    /* Determine the row r that contains n.
       Row r starts at index 2^r - 1 and has length 2^r. */
    idx_t r = 63 - __builtin_clzll((unsigned long long)(n + 1));  /* floor(log2(n+1)) */
    idx_t start = ((idx_t)1 << r) - 1;          /* 2^r - 1 */
    idx_t offset = n - start;                  /* position inside the row (0‑based) */

    /* Scan the values 1..r+1, each occurring C(r, k-1) times. */
    idx_t binom = 1;           /* C(r,0) */
    for (idx_t k = 1; k <= r + 1; ++k) {
        if (offset < binom)        /* found the value */
            return (seq_t)k;
        offset -= binom;
        if (k <= r) {              /* compute next binomial coefficient */
            binom = binom * (r - k + 1) / k;
        }
    }
    return (seq_t)1;  /* unreachable, but keeps the compiler happy */
}
