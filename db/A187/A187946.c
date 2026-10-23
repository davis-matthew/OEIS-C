/*
 * A187946  [nr+kr]-[nr]-[kr], where r=(1+sqrt(5))/2, k=5, [ ]=floor.
 * https://oeis.org/A187946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r + 5*r) - floor(n*r) - floor(5*r), r =
 *            (1+sqrt(5))/2, k = 5 Since floor(5*r)=8, the value is 1 iff the
 *            fractional part of n*r is greater than 1 - 0.0901699… (the
 *            fractional part of 5*r)
 * Range:     u8 none, u16 none, u32 n <= 7, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t BASE     = 1000000000000000000ULL;          /* 10^18 */
static const seq_t SQRT5_F  = 2236067977499789696ULL;         /* floor(sqrt(5)*BASE) */
static const seq_t PHI_F    = (BASE + SQRT5_F) / 2;           /* floor(phi*BASE)   */
static const seq_t THRESH   = 909830056250525760ULL;          /* BASE - (5*PHI_F)%BASE */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1, but safe return */

    /* product = n * phi_f (exact in 128-bit) */
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)PHI_F;

    /* f_num = fractional part of n*phi represented as integer in [0,BASE) */
    seq_t f_num = (seq_t)(prod % BASE);

    return (f_num > THRESH) ? (seq_t)1 : (seq_t)0;
}
