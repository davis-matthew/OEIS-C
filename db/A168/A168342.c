/*
 * A168342  Even atomic numbers in the Janet table of the PSE, read right to
 *          left along rows.
 * https://oeis.org/A168342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, tabf
 * Formula:   a(n) = the n-th even atomic number appearing when the integers *
 *            1,2,3,... are arranged in the Janet table of the periodic table.
 *            * The rows have sizes 2,2,8,8,18,18,32,32,..., i.e. twice 2*k*k
 *            for k=1,2,3,... * Within each row the numbers are read
 *            right‑to‑left (descending). Only the * even numb...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* keep the cumulative count of even numbers produced so far */
    idx_t count = 0;

    /* current start integer of the next row */
    idx_t start = 1;

    /* current k (size parameter) and index of row within the pair (0 or 1) */
    idx_t k = 1;
    uint8_t row_in_k = 0;

    while (1) {
        /* compute size of this row: 2*k*k */
        idx_t size = 2 * k * k;
        idx_t end = start + size - 1;

        /* find the first and last even numbers in this inclusive range */
        idx_t first_even = (start % 2 == 0) ? start : start + 1;
        if (first_even <= end) {
            idx_t last_even = (end % 2 == 0) ? end : end - 1;
            /* number of evens in this row */
            idx_t num_ev = (last_even - first_even) / 2 + 1;

            /* if the target n lies in this row */
            if (count + num_ev >= n) {
                /* offset from the largest even when reading right-to-left */
                idx_t offset = n - count - 1;          /* 0‑based within the row */
                seq_t val = (seq_t)(last_even - 2 * offset);
                return val;
            }
            /* otherwise, skip this row */
            count += num_ev;
        }

        /* advance to next row */
        start = end + 1;
        row_in_k++;
        if (row_in_k == 2) {          /* both rows of this k have been processed */
            row_in_k = 0;
            k++;
        }
    }
}
