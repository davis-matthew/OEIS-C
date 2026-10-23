/*
 * A127985  a(n) = floor(2^n*(n/3 + 4/9)).
 * https://oeis.org/A127985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2^n*(n/3 + 4/9))
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=4, a(3)=11, a(n)=4a(n-1)-3a(n-2)-4a(n-3)+4a(n-4) for n≥4 */

#include <stdint.h>
#include <stddef.h>

/* Compute the sequence value a(n) for any n ≥ 0.
   The calculation is performed using the closed‑form formula
   a(n) = floor( 2^n * ( (3*n + 4) / 9 ) ).
   For small integer types (≤64 bits) a 128–bit intermediate is used
   to avoid overflow.  For larger or arbitrary‑precision types the
   operations are performed directly on seq_t.  */
seq_t a(idx_t n)
{
    /* Compute 2^n in seq_t by repeated doubling. */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        power *= (seq_t)2;

    /* Compute (3*n + 4) */
    if (sizeof(seq_t) <= 8) {
        /* Use 128‑bit intermediate for 64‑bit builds. */
        __int128 mult   = (__int128)3 * (idx_t)n + 4;
        __int128 temp   = (__int128)power * mult;
        return (seq_t)(temp / 9);
    } else {
        /* Arbitrary‑precision build: use seq_t operators directly. */
        seq_t mult  = (seq_t)3 * (seq_t)n + (seq_t)4;
        seq_t temp  = power * mult;
        return temp / 9;
    }
}
