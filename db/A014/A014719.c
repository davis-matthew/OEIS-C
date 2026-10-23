/*
 * A014719  Squares of elements in Pascal triangle (by row) that are not 1.
 * https://oeis.org/A014719
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A014410(n)^2 (square of the (n‑2)th non‑1 entry of
 *            Pascal’s triangle)
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 161, u64 n <= 578,
 *            u128 n <= 2242, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* undefined for n < 2 */

    /* zero‑based index within the flattened list of non‑1 entries */
    idx_t target = n - 2;
    idx_t r = 2;            /* current row (row 2 is the first with a non‑1 entry) */
    idx_t cumulative = 0;   /* number of non‑1 entries before the current row */

    /* locate the row that contains the target index */
    while (cumulative + (r - 1) <= target)
    {
        cumulative += (r - 1);
        ++r;
    }

    /* position of the target within its row (0‑based) */
    idx_t pos_in_row = target - cumulative;
    /* binomial coefficient index j (1‑based) */
    idx_t j = pos_in_row + 1;

    /* compute C(r, j) multiplicatively using seq_t arithmetic */
    seq_t comb = 1;
    for (idx_t i = 1; i <= j; ++i)
    {
        comb *= (seq_t)(r - j + i);
        comb /= (seq_t)i;
    }

    /* return the square of the binomial coefficient */
    return comb * comb;
}
