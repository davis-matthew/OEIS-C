/*
 * A094497  Triangular table A(n,j) = C(n,j) - C(n,j) mod n^3, difference of
 *          binomial coefficient and its residue mod n^3, read by rows.
 * https://oeis.org/A094497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (C(r,j) - C(r,j) mod r^3), where r is the unique
 *            triangular row index with r(r+1)/2 ≤ n < (r+1)(r+2)/2 and j = n
 *            - r(r+1)/2. One simply computes the binomial coefficient exactly
 *            in 128‑bit arithmetic, then truncates it to the largest multiple
 *            of r^3
 * Range:     u8 n <= 110, u16 n <= 197, u32 n <= 646, u64 n <= 2376,
 *            u128 n <= 8840, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case a(0) */
    if (n == 0) return (seq_t)1;

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    for (;; r++)
    {
        __int128 next = (__int128)(r + 1) * (r + 2) / 2;
        if (next > n) break;
    }

    /* Index j inside row r */
    idx_t prev_total = r * (r + 1) / 2;
    idx_t j = n - prev_total;

    /* Compute binomial coefficient C(r,j) exactly in 128‑bit arithmetic. */
    idx_t k = j;
    if (k > r - k) k = r - k;          /* use symmetry for fewer iterations */

    __int128 bin128 = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        bin128 = bin128 * ( (__int128)(r - k + i) ); /* multiply by numerator part */
        bin128 = bin128 / i;                          /* exact division by denominator */
    }

    /* Compute r³ in 128‑bit to avoid overflow. */
    __int128 r3_128 = (__int128)r * r * r;

    /* Truncate bin to the largest multiple of r³. */
    const __int128 q  = bin128 / r3_128;      /* floor(bin / r³) */
    const __int128 res128 = q * r3_128;      /* diff = q * r³ */

    return (seq_t)res128;
}
