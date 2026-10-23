/*
 * A145362  Lower triangular array S1hat(-1) read by rows, related to
 *          partition number array A145361.
 * https://oeis.org/A145362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = 1 if the term lies in the lower triangular portion of its row,
   otherwise 0.  Row r is the unique integer with triangular(r‑1) < n ≤ triangular(r). */

static inline idx_t triangular(idx_t r) {
    /* Compute r*(r+1)/2 without overflow */
    if (r == 0) return 0;
    if ((r & 1) == 0) {
        return (r / 2) * (r + 1);
    } else {
        return ((r + 1) / 2) * r;
    }
}

static inline idx_t find_row(idx_t n) {
    /* Minimal r with triangular(r) ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* safety, n is defined ≥1 */
    idx_t r = find_row(n);
    idx_t prev = triangular(r - 1);  /* terms before this row */
    idx_t pos = n - prev;            /* 1‑based position in the row (1 ≤ pos ≤ r) */
    idx_t threshold = (r + 1) / 2;   /* ceil(r/2) */
    return (pos >= threshold) ? (seq_t)1 : (seq_t)0;
}
