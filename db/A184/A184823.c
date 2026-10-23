/*
 * A184823  a(n) = n + floor(n/t) + floor(n/t^2) + floor(n/t^3), where t is
 *          the tetranacci constant.
 * https://oeis.org/A184823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/t) + floor(n/t^2) + floor(n/t^3) where t is
 *            the tetranacci constant satisfying t = 1 + 1/t + 1/t^2 + 1/t^3
 * Range:     u8 n <= 117, u16 n <= 117, u32 n <= 117, u64 n <= 117,
 *            u128 n <= 117, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static const long double TETRA_CONST  = 1.9275619754829253042619058L;   /* t       */
static const long double TETRA_CONST2 = TETRA_CONST * TETRA_CONST;      /* t^2     */
static const long double TETRA_CONST3 = TETRA_CONST2 * TETRA_CONST;     /* t^3     */

seq_t a(idx_t n)
{
    uint64_t un = n;                               /* n fits in 64‑bit */
    /* compute the three floor terms using long double arithmetic */
    uint64_t f1 = (uint64_t)floorl((long double)un / TETRA_CONST);
    uint64_t f2 = (uint64_t)floorl((long double)un / TETRA_CONST2);
    uint64_t f3 = (uint64_t)floorl((long double)un / TETRA_CONST3);

    /* accumulate the result in the target integer type */
    seq_t res = (seq_t)un;
    res += (seq_t)f1;
    res += (seq_t)f2;
    res += (seq_t)f3;
    return res;
}
