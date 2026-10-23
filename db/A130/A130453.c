/*
 * A130453  A097806 * A059268.
 * https://oeis.org/A130453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^{min(j, r-1)} where r is the unique row number with
 *            (r-1)·r/2 < n ≤ r(r+1)/2, and j = n - (r-1)·r/2
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the OEIS sequence A130453. */
seq_t a(idx_t n)
{
    /* Find the row number r satisfying r(r+1)/2 >= n.  Use binary
       search with 128‑bit arithmetic to avoid overflow. */
    idx_t low = 0, high = 1;
    /* Increase high until it is large enough. */
    while (((__int128)high * (high + 1) / 2) < n) {
        high <<= 1;
        if (high == 0) {          /* wrap around -> n is max uint64_t */
            high = n;             /* ensures termination */
            break;
        }
    }
    /* Binary search between low and high (inclusive). */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                           /* r ≥ 1 */

    /* Column index j (1‑based) within the row. */
    idx_t j = n - ( (__int128)(r - 1) * r / 2 );

    /* Exponent m = min(j, r-1).   Recall that j ∈ [1, r]. */
    idx_t m = (j <= r - 1) ? j : r - 1;

    /* Return 2^m.  Use seq_t shifts; shift count fits in seq_t. */
    seq_t value = (seq_t)1;
    value <<= m;
    return value;
}
