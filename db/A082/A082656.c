/*
 * A082656  Trajectory of 39 under map x -> x/2 if x even, 3x+9 if x odd.
 * https://oeis.org/A082656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 39; for n > 1, a(n) = a(n-1)/2 if a(n-1) is even,
 *            otherwise a(n-1) = 3·a(n-1)+9 (trajectory of the starting value
 *            39 under the map x→x/2 (x even) or x→3x+9 (x odd))
 * Range:     u8 n <= 5, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* idx_t is an unsigned 64‑bit integer, seq_t may be any unsigned integer type */
    seq_t cur = (seq_t)39;            /* a(1) */
    if (n <= 1) return cur;

    /* iterate the map n‑1 times */
    for (idx_t i = 2; i <= n; ++i)
    {
        if (cur & 1)            /* odd */
            cur = cur * 3 + 9;
        else                    /* even */
            cur = cur / 2;
    }
    return cur;
}
