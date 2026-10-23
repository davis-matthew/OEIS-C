/*
 * A177878  Triangle in which row n is generated from (1,2,3,...,n) dot (n,
 *          n-1,...,1) with subtractive carryovers.
 * https://oeis.org/A177878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten of rows produced by the subtractive–carry dot
 *            product of (1,2,…,r+1) and (r+1,…,2,1), where r =
 *            floor((√(8n+1)-1)/2). Each row r has length r+1; a(n) is the
 *            (n‑offset(r))‑th element of that row
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find row r such that offset(r) <= n < offset(r+1) */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;               /* 0‑based index inside the row */

    seq_t prev = 0;
    for (idx_t i = 0; i <= k; ++i) {
        seq_t ai = (seq_t)(i + 1);
        seq_t bi = (seq_t)(r + 1 - i);
        seq_t prod = ai * bi;
        seq_t ci = (i == 0) ? prod : prod - prev;
        prev = ci;
    }
    return prev;
}
