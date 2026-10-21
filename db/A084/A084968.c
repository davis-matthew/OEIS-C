/*
 * A084968  Multiples of 7 coprime to 30.
 * https://oeis.org/A084968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = base[(n‑1) mod 8] + 210 * floor((n‑1)/8) where base =
 *            {7,49,77,91,119,133,161,203} (multiples of 7 coprime to 30)
 * Range:     u8 n <= 9, u16 n <= 2497, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t base[8] = {7,49,77,91,119,133,161,203};

    idx_t off = n - 1;              /* zero‑based index of the term */
    idx_t q   = off / 8;            /* full periods of length 8 */
    idx_t r   = off % 8;            /* position within the period */

    return (seq_t)(base[r] + 210 * q);
}
