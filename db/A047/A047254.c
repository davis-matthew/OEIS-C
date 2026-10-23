/*
 * A047254  Numbers that are congruent to {2, 3, 5} mod 6.
 * https://oeis.org/A047254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 6k-1, a(3k-1) = 6k-3, a(3k-2) = 6k-4 (or equivalently
 *            numbers ≡ {2,3,5} (mod 6), offset 1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based and n >= 1 */
    idx_t m = n - 1;           /* zero‑based offset */
    idx_t block = m / 3;       /* number of complete periods of 3 */
    idx_t rem   = m % 3;       /* 0 => first residue, 1 => second, 2 => third */

    /* compute 6 * block in the seq_t type */
    seq_t base = (seq_t)6 * (seq_t)block;

    /* add the appropriate small offset depending on the residue */
    if (rem == 0)
        return base + (seq_t)2;   /* 6k – 4  -> 6k-4 + 2 = 6k-2 -> 2 (mod 6) */
    else if (rem == 1)
        return base + (seq_t)3;   /* 6k – 3  -> 6k-3 + 0 = 6k-3 -> 3 (mod 6) */
    else
        return base + (seq_t)5;   /* 6k – 1  -> 6k-1 + 0 = 6k-1 -> 5 (mod 6) */
}
