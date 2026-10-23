/*
 * A158944  Triangle by columns: the natural numbers interleaved with zeros in
 *          every column: (1, 0, 2, 0, 3, 0, 4, ...)
 * https://oeis.org/A158944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = ( (r - j) / 2 + 1 ) if (r - j) is even, otherwise 0,
 *            where r is the unique integer with (r+1)(r+2)/2 ≥ n and j = n -
 *            r(r+1)/2 - 1
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* n is assumed ≥ 1 in the OEIS definition */

    /* Find the row r (0‑based) containing index n. */
    idx_t r = 0;
    while ( ((r + 1u) * (r + 2u)) / 2u < n )
        r++;

    /* Compute previous cumulative count (row r‑1). */
    idx_t prev = r * (r + 1u) / 2u;

    /* Column index j within this row (0‑based). */
    idx_t j = n - prev - 1u;

    /* Difference r – j = n – k in the OEIS formula. */
    idx_t delta = r - j;

    /* If delta is odd → 0; if even → (delta/2 + 1). */
    if ( (delta & 1u) == 0u )
        return (seq_t)(delta >> 1u) + 1u;
    else
        return (seq_t)0;
}
