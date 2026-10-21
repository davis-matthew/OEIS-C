/*
 * A069497  Triangular numbers of the form 6*k.
 * https://oeis.org/A069497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T_m = m*(m+1)/2 where m = 12*((n-1)/4)+R[(n-1)%4] and R =
 *            {0,3,8,11}
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence enumerates triangular numbers divisible by 6. */

seq_t a(idx_t n)
{
    /* zero‑based indexing of the triangular indices : 0,3,8,11,12,15,20,... */
    static const idx_t R[4] = {0, 3, 8, 11};

    idx_t k = n;                     /* 1‑based sequence index */
    idx_t q = (k - 1) / 4;           /* complete blocks of four */
    idx_t r = (k - 1) % 4;           /* offset within the block */
    idx_t m_idx = 12 * q + R[r];     /* actual triangular index */

    seq_t m = (seq_t)m_idx;          /* cast to seq_t for arithmetic */
    return m * (m + 1) / 2;          /* triangular number */
}
