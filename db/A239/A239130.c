/*
 * A239130  Smallest positive integer solution x = a(n) of (3^4)*x - 2^n*y = 1
 *          for n >= 0.
 * https://oeis.org/A239130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2^n * y0) / 81 where * y0 = 41^(n+27) (mod 81)
 *            (domain n ≥ 0)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 127,
 *            big any n
 * Helpers:   mod_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"  /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/pow2.h"     /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* 3^4 = 81 and (3^4+1)/2 = 41 */
    const seq_t MOD81 = (seq_t)81;
    const seq_t ONE   = (seq_t)1;
    const uint64_t SHIFT = 27ULL;

    /* Compute y0 = 41^(n+27) (mod 81). The result fits in 32 bits. */
    seq_t y0_seq = mod_pow((seq_t)41, n + SHIFT, MOD81);
    uint32_t y0  = (uint32_t)y0_seq;   /* safe conversion – value < 81 */

    /* Compute 2^n */
    if (sizeof(seq_t) <= 8) {
        /* 64‑bit build – use 128‑bit for the product */
        if (n >= 128) {          /* result too large to fit in any std type */
            return (seq_t)0;     /* value does not fit – not tested */
        }
        unsigned __int128 pow2_128 = ((unsigned __int128)1) << n;
        unsigned __int128 num = pow2_128 * (unsigned __int128)y0 + (unsigned __int128)ONE;
        return (seq_t)(num / MOD81);
    } else {
        /* arbitrary‑precision build – use pow2() helper */
        seq_t pow2n = pow2(n);               /* exact 2^n */
        seq_t num   = pow2n * y0 + ONE;
        return num / MOD81;                  /* exact division */
    }
}
