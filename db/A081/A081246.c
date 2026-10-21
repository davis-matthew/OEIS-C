/*
 * A081246  Triangle in which (2^n+1)-st row gives trajectory of x=2^n+1 under
 *          the map x -> x/2 if x is even, x -> x+1 if x is odd, stopping when
 *          reaching 1.
 * https://oeis.org/A081246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = the element of order i in the trajectory of x = 2^r+1,
 *            where r is the smallest integer with r^2+3r >= n and i =
 *            n-(r-1)^2-3(r-1)-1. Trajectory follows the map: if x even → x/2
 *            else → x+1, stopping at 1
 * Range:     u8 n <= 70, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row number r so that r^2+3r ≥ n. */
    idx_t r = 0;
    while (r * r + 3 * r < n)
        ++r;

    /* Number of terms up to the previous row. */
    idx_t prev = 0;
    if (r > 0)
        prev = (r - 1) * (r - 1) + 3 * (r - 1);

    /* Position inside the current row (0‑based). */
    idx_t pos = n - prev - 1;

    /* Starting value for the trajectory. */
    seq_t x = (((seq_t)1) << r) + 1;

    /* Advance pos steps. */
    while (pos--) {
        if ((x & (seq_t)1) == 0) /* even */
            x = x >> 1;
        else
            x = x + 1;
    }
    return x;
}
