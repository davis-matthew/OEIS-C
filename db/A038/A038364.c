/*
 * A038364  Numbers n such that n = (product of digits of n) + (sum of digits
 *          of n).
 * https://oeis.org/A038364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(1) = 0; for 2 ≤ n ≤ 10, a(n) = 10·(n–1) + 9; otherwise a(n) =
 *            0
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the sequence of numbers n such that n = product of digits of n
   + sum of digits of n.  The only solutions are 0 and the two‑digit numbers
   ending in 9 (19,29,…,99). */

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;          /* the single 0 is the first term */

    if (n >= 2 && n <= 10) {
        seq_t val = (seq_t)(n - 1); /* (n-1) is at most 9, safe to cast */
        val = val * 10 + 9;         /* build 10·(n-1)+9 */
        return val;
    }

    return 0; /* terms beyond the known finite set */
}
