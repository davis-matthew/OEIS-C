/*
 * A028734  Nonsquares mod 21.
 * https://oeis.org/A028734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Non‑residues modulo 21.
   a(n) = 21 * floor((n-1)/13) + r[(n-1) % 13],
   where r = [2, 3, 5, 6, 8, 10, 11, 12, 13, 14, 17, 19, 20] */

seq_t a(idx_t n)
{
    static const seq_t r[13] = {2, 3, 5, 6, 8, 10, 11, 12, 13, 14, 17, 19, 20};

    if (n == 0) return (seq_t)0;    /* offset is 1; undefined below */

    idx_t k = n - 1;                 /* zero‑based position in the infinite list */
    idx_t block   = k / 13;          /* how many complete 21‑element blocks */
    idx_t index   = k % 13;          /* position within the block */

    return (seq_t)21 * (seq_t)block + r[index];
}
