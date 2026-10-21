/*
 * A052460  3-magic series constant.
 * https://oeis.org/A052460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3/4 * (n^2 + 1)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 28, u64 n <= 689,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset starts at 1, but guard for safety */

    /* Use 128‑bit integer for intermediate calculations.
       This guarantees no overflow for n up to about 10⁴
       (a(10 000) ≈ 2.5 × 10¹⁹) and truncates cleanly
       for the 64‑bit build when the result fits. */
    unsigned __int128 nn   = n;           /* n as 128‑bit */
    unsigned __int128 n2   = nn * nn;     /* n² */
    unsigned __int128 n3   = nn * n2;     /* n³ */
    unsigned __int128 n2p1 = n2 + 1;      /* n² + 1 */
    unsigned __int128 sq    = n2p1 * n2p1;/* (n² + 1)² */
    unsigned __int128 prod  = n3 * sq;    /* n³ * (n² + 1)² */
    prod /= 4;                           /* divide by 4 */
    return (seq_t)prod;                  /* cast back to seq_t */
}
