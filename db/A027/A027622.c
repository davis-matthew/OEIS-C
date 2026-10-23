/*
 * A027622  a(n) = n + (n+1)^2 + (n+2)^3 + (n+3)^4 + (n+4)^5.
 * https://oeis.org/A027622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + (n+1)^2 + (n+2)^3 + (n+3)^4 + (n+4)^5
 * Range:     u8 none, u16 n <= 5, u32 n <= 80, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit accumulator to avoid overflow for the 64‑bit build
       while keeping the result exact for all arguments that fit in seq_t. */
    unsigned __int128 base = n;          /* promote n to 128‑bit */
    unsigned __int128 t2 = (base + 1) * (base + 1);
    unsigned __int128 t3 = (base + 2) * (base + 2) * (base + 2);
    unsigned __int128 t4 = (base + 3) * (base + 3) * (base + 3) * (base + 3);
    unsigned __int128 t5 = (base + 4) * (base + 4) * (base + 4) * (base + 4) * (base + 4);
    unsigned __int128 sum = base + t2 + t3 + t4 + t5;
    return (seq_t)sum;
}
