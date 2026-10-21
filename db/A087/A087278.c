/*
 * A087278  Nonnegative integers whose distance to the nearest square is not
 *          greater than 1.
 * https://oeis.org/A087278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3) * (floor(n/3)+2) + (n mod 3)
 * Range:     u8 n <= 45, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (equivalent to the piecewise definition in the OEIS entry) */
seq_t a(idx_t n)
{
    /* compute k = floor(n/3) and r = n mod 3 */
    idx_t k = n / 3;
    idx_t r = n - 3 * k;          /* same as n % 3 */

    /* use 128‑bit intermediate to avoid overflow when seq_t is 64‑bit */
    unsigned __int128 prod = (unsigned __int128)k * ((unsigned __int128)k + 2);
    prod += r;

    return (seq_t)prod;
}
