/*
 * A189400  n+[nr/s]+[nt/s]; r=1, s=sqrt(e), t=e.
 * https://oeis.org/A189400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A189400
    Formula: a(n) = n + ⌊n / √e⌋ + ⌊n * √e⌋
    r = 1, s = √e, t = e
*/

#include <stdint.h>

static const unsigned long long SQRT_E_NUM = 1648721270700128146ULL; /* floor(√e * 10^18) */
static const unsigned long long SQRT_E_DEN = 1000000000000000000ULL; /* 10^18          */

seq_t a(idx_t n)
{
    unsigned __int128 n128 = (unsigned __int128)n;

    /* ⌊n * √e⌋  = ⌊n * SQRT_E_NUM / SQRT_E_DEN⌋ */
    unsigned __int128 prod1 = n128 * SQRT_E_NUM;
    unsigned long long term1 = (unsigned long long)(prod1 / SQRT_E_DEN);

    /* ⌊n / √e⌋  = ⌊n * SQRT_E_DEN / SQRT_E_NUM⌋ */
    unsigned __int128 prod2 = n128 * SQRT_E_DEN;
    unsigned long long term2 = (unsigned long long)(prod2 / SQRT_E_NUM);

    unsigned __int128 sum = n128 + term1 + term2;
    return (seq_t)sum;
}
