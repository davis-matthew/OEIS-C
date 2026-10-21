/*
 * A072833  Numbers that are congruent to 0, 5, 8, 9 mod 12.
 * https://oeis.org/A072833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/4)*12 + {0,5,8,9}[n%4]
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offsets for each remainder class modulo 12 in the order 0,5,8,9 */
    static const seq_t offsets[4] = {0,5,8,9};

    /* number of complete 12‑blocks before the current block */
    idx_t k = n / 4;
    /* position inside the current 12‑block (0..3) */
    idx_t r = n % 4;

    /* compute the result in seq_t type */
    return (seq_t)k * 12 + offsets[r];
}
