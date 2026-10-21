/*
 * A097298  Eighth column (m=7) of (1,6)-Pascal triangle A096956.
 * https://oeis.org/A097298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+42)*binomial(n+6,6)/7 (equivalently a(n) =
 *            6*binomial(n+7,7) - 5*binomial(n+6,7) )
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 72, u64 n <= 1901,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediate to avoid overflows on 64‑bit builds.
       The result is then converted back to seq_t which may be 8,16,32,64,
       128 bits or an arbitrary‑precision integer type. */
    unsigned __int128 binom = 1;
    for (idx_t k = 1; k <= 6; ++k) {
        binom = binom * (n + k) / k;
    }
    unsigned __int128 result = binom * (n + 42) / 7;
    return (seq_t)result;
}
