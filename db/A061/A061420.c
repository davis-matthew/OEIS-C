/*
 * A061420  a(n) = a(ceiling((n-1)*2/3)) + 1 with a(0) = 0.
 * https://oeis.org/A061420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor(2*n/3)) + 1, a(0) = 0 The recurrence reduces n
 *            roughly by a factor 3/2 each step. An iterative implementation
 *            uses 128‑bit arithmetic for the product 2*n, which prevents
 *            overflow on 64‑bit indices
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n > 0)
    {
        /* compute floor(2*n/3) without overflow */
        __int128 prod = (__int128)n * 2;      /* 2*n in 128 bits */
        n = (idx_t)(prod / 3);                 /* floor division by 3 */
        ++count;
    }
    return count;
}
