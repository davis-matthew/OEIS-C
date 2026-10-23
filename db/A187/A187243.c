/*
 * A187243  Number of ways of making change for n cents using coins of 1, 5,
 *          and 10 cents.
 * https://oeis.org/A187243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((floor(n/5)+2)^2 / 4)
 * Range:     u8 n <= 149, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* number of partitions of n into parts 1, 5 and 10   */
seq_t a(idx_t n)
{
    /* t = floor(n/5)+2 */
    idx_t t_idx = n / 5 + 2;

    /* use a wide type for the multiplication to avoid overflow */
    unsigned __int128 t = (unsigned __int128)t_idx;
    unsigned __int128 prod = t * t;        /* (floor(n/5)+2)^2   */
    seq_t result = (seq_t)(prod / 4);      /* divide by 4, truncate */

    return result;
}
