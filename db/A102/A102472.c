/*
 * A102472  Triangle read by rows. Let S(k) be the sequence defined by F(0)=0,
 *          F(1)=1, F(n-1) + (n+k)*F(n) = F(n+1). E.g. S(0) = 0, 1, 1, 3, 10,
 *          43, 225, 1393, 9976, 81201, ... Then S(0), S(1), S(2), ... are
 *          written vertically, next to each other, with the initial term of
 *          each on the next row down.
 * https://oeis.org/A102472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows constructed from the sequence S(k)
   defined by F(0)=0, F(1)=1, and  F(n-1) + (n+k)*F(n) = F(n+1)
   for n≥1.  a(n) is obtained by reading the infinite triangle
   row‑by‑row (OEIS A102472). */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 1, but keep harmless */

    /* Find the row r such that r*(r+1)/2 ≥ n and (r-1)*r/2 < n. */
    idx_t r = 0;
    while (((__int128)r * (r + 1)) / 2 < n)
        ++r;
    idx_t prev = (idx_t)(((__int128)(r - 1) * r) / 2);   /* elements before row r */

    /* Zero‑based index inside the row. */
    idx_t idx0 = n - prev - 1;       /* 0 … r-1 */
    idx_t k   = idx0;                /* the parameter of S(k) */
    idx_t m   = r - idx0;            /* the index inside S(k) (1‑based) */

    /* Compute F_m for the given k using the recurrence. */
    seq_t prev2 = 0;   /* F0 */
    seq_t prev1 = 1;   /* F1 */
    if (m == 1)
        return prev1;      /* F1 = 1 */
    /* Iterate for i = 1 .. m-1 to obtain F_{i+1}. */
    for (idx_t i = 1; i < m; ++i) {
        seq_t next = prev2 + ((seq_t)i + k) * prev1;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;       /* F_m */
}
