/*
 * A018087  Powers of fourth root of 15 rounded down.
 * https://oeis.org/A018087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(15^(n/4))
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow15_q
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow15_q.h"

/* sequence value */
seq_t a(idx_t n)
{
    /* initial values a(0) … a(3) */
    static const seq_t init[4] = {1, 1, 3, 7};
    if (n < 4)
        return init[n];

    /* split n into q = n/4 and r = n%4 */
    idx_t  q = n / 4;
    int    r = (int)(n % 4);

    /* base = 15^q */
    seq_t base = pow15_q(q);

    /* constants for the 4 fourth‑root parts */
    /* shift K = 63 is large enough to keep error <1 for all base values that fit in seq_t */
    static const unsigned __int128 SCALE  = ((unsigned __int128)1) << 63;
    static unsigned __int128            C[4];
    static int                       init_c = 0;
    if (!init_c) {
        C[0] = SCALE;
        C[1] = (unsigned __int128)
               (floor(pow(15.0, 0.25) * (double)SCALE));
        C[2] = (unsigned __int128)
               (floor(pow(15.0, 0.50) * (double)SCALE));
        C[3] = (unsigned __int128)
               (floor(pow(15.0, 0.75) * (double)SCALE));
        init_c = 1;
    }

    /* compute floor(base * C[r] / 2^63) */
    unsigned __int128 prod = (unsigned __int128)base * C[r];
    seq_t result = (seq_t)(prod >> 63);

    return result;
}
