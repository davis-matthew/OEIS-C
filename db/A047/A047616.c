/*
 * A047616  Numbers that are congruent to {0, 1, 5} mod 8.
 * https://oeis.org/A047616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/3) + {0,1,5}[(n-1) mod 3]
 * Range:     u8 n <= 96, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;                  /* sequence defined for n ≥ 1 */

    seq_t k = (seq_t)n - 1;               /* zero‑based index: 0,1,2,… */
    seq_t q = k / 3;                      /* number of full 3‑element blocks */
    idx_t r = (idx_t)(k % 3);             /* position within current block (0–2) */

    static const seq_t rem[3] = {0, 1, 5};
    return rem[r] + q * (seq_t)8;
}
