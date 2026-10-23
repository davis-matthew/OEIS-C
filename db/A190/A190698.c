/*
 * A190698  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),4,1) and [
 *          ]=floor.
 * https://oeis.org/A190698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(4n+1)√3⌋ - 4·⌊n√3⌋ - ⌊1·√3⌋
 * Range:     u8 none, u16 n <= 18, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For sqrt(3) the integer–square‑root trick can be used: */
/*   ⌊k√3⌋ = ⌊√(3k²)⌋  */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t sn = (seq_t)n;

    /* compute ⌊(4n+1)√3⌋ = ⌊√(3(4n+1)²)⌋ */
    seq_t temp = sn * 4 + 1;            /* 4n+1 */
    seq_t m1 = 3 * temp * temp;        /* 3(4n+1)² */
    seq_t s1 = floor_sqrt(m1);         /* ⌊(4n+1)√3⌋ */

    /* compute ⌊n√3⌋ = ⌊√(3n²)⌋ */
    seq_t m2 = 3 * sn * sn;            /* 3n² */
    seq_t s2 = floor_sqrt(m2);         /* ⌊n√3⌋ */

    /* floor(1·√3) = 1, so a(n) = s1 - 4·s2 - 1 */
    return s1 - 4 * s2 - 1;
}
