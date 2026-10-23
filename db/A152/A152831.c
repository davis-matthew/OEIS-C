/*
 * A152831  Triangle read by rows, A007318 repeated five times .
 * https://oeis.org/A152831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = C(r, k) where r is the row index of Pascal's triangle
 *            (A007318) that appears five times in the stream. r is chosen so
 *            that cumulative_count(r‑1) ≤ n < cumulative_count(r), with
 *            cumulative_count(r) = 5*(r+1)*(r+2)/2. k = (n -
 *            cumulative_count(r‑1)) mod (r+1) (0‑based)
 * Range:     u8 n <= 333, u16 n <= 957, u32 n <= 3165, u64 n <= 6374,
 *            u128 n <= 6374, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_value(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Use multiplication before division to keep the
           intermediate result integral.  Cast operands to
           seq_t to avoid overflow in the intermediate
           mult‑by‑division step when seq_t is 128‑bit
           or arbitrary‑precision. */
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains the nth element. */
    idx_t r = 0;
    idx_t cum = 0;          /* number of elements processed so far */
    while (1) {
        idx_t row_size = 5 * (r + 1);          /* 5 copies of row r */
        if (cum + row_size > n)
            break;
        cum += row_size;
        ++r;
    }

    /* Position within the block of 5 copies of row r. */
    idx_t offset_in_row = n - cum;
    idx_t k = offset_in_row % (r + 1);          /* column index */

    return binom_value(r, k);
}
