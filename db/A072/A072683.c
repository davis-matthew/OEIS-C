/*
 * A072683  Numbers k such that the last digit of F(k) is 3 where F(k) is the
 *          k-th Fibonacci number.
 * https://oeis.org/A072683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 60*⌊(n-1)/8⌋ + r[(n-1) mod 8] where r =
 *            {4,7,13,26,44,46,47,53} (indices of Fibonacci numbers ending in
 *            3)
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset: a(1) = 4 */
    if (n == 0)
        return (seq_t)0;           /* defensive, not used by OEIS */

    const idx_t block_size   = 8;   /* number of residues per 60‑period */
    const idx_t period       = 60;  /* Pisano period of Fibonacci mod 10 */

    /* Residues of F(k) mod 10 = 3 (k indexed from 1) */
    static const seq_t residues[8] = {
        (seq_t)4,  (seq_t)7,  (seq_t)13, (seq_t)26,
        (seq_t)44, (seq_t)46, (seq_t)47, (seq_t)53
    };

    idx_t idx = n - 1;          /* 0‑based position in the infinite sequence */
    idx_t block = idx / block_size;   /* which 60‑period block */
    idx_t pos   = idx % block_size;   /* position within the block */

    seq_t value = (seq_t)block * (seq_t)period + residues[pos];
    return value;
}
