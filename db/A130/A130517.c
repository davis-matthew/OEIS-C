/*
 * A130517  Triangle read by rows: row n counts down from n in steps of 2,
 *          then counts up the remaining elements in the set {1,2,...,n},
 *          again in steps of 2.
 * https://oeis.org/A130517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 7260, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Code: computes a(n) of OEIS A130517 (the “triangle read by rows”).
   The triangle is defined row‑by‑row: for row r
        – if r is even: r, r‑2, …, 2, 1, 3, 5, …
        – if r is odd : r, r‑2, …, 1, 2, 4, 6, …
   This routine finds the row containing the nth entry and returns the
   corresponding value in the row. */
seq_t a(idx_t n)
{
    /* Find the smallest r such that r*(r+1)/2 >= n.  The loop runs
       at most about √(2n) times; for n ≤ 10 000 this is < 150. */
    idx_t r = 1;
    while ((__int128)r * (r + 1) / 2 < n)
        ++r;

    /* Offset of the first element in row r.  Triangular number of
       (r-1) plus one. */
    idx_t offset = ((r - 1) * r) / 2 + 1;
    /* Position k within the row (1‑based). */
    idx_t k = n - offset + 1;   /* k = n - ((r-1)*r)/2 */

    /* Compute the value at position k of row r. */
    idx_t val;
    if ((r & 1U) == 0) {                 /* even r: evens descending, odds ascending */
        idx_t half = r / 2;
        if (k <= half) {
            val = r - 2 * (k - 1);
        } else {
            idx_t t = k - half;
            val = 2 * t - 1;
        }
    } else {                             /* odd r: odds descending, evens ascending */
        idx_t half = (r + 1) / 2;
        if (k <= half) {
            val = r - 2 * (k - 1);
        } else {
            idx_t t = k - half;
            val = 2 * t;
        }
    }

    return (seq_t)val;
}
