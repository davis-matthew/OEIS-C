/*
 * A021573  Decimal expansion of 1/569.
 * https://oeis.org/A021573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit (0‑based) of the decimal expansion of
 *            1/569
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) without allocating any additional memory. */
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t rem = 1;           /* remainder before the first division (1/569) */
    seq_t digit = 0;         /* current digit being produced */

    /* Perform long‑division steps up to the desired index. */
    for (idx_t i = 0; i <= n; ++i) {
        idx_t prod  = rem * 10ULL;          /* scaled remainder */
        digit = (seq_t)(prod / 569ULL);     /* next digit 0‑9 */
        rem   = prod % 569ULL;              /* update remainder */
    }

    return digit;
}
