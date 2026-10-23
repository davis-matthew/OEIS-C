/*
 * A042836  Numerators of continued fraction convergents to sqrt(949).
 * https://oeis.org/A042836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of convergents to sqrt(949) from continued fraction.
   The continued fraction of sqrt(949) has period 27 with partial
   quotients:
   30; [1,4,6,1,1,1,4,2,14,1,19,1,1,1,1,19,1,14,2,4,1,1,1,6,4,1,60].
   Here a(0)=30 and a(n) = p_n where
      p_{-2}=0, p_{-1}=1,
      p_k = a_k * p_{k-1} + p_{k-2}  (k ≥ 0).
   Offsets: a(0) is the first term. */

#include <stdint.h>

static const seq_t period[27] = {
    1ULL, 4ULL, 6ULL, 1ULL, 1ULL, 1ULL, 4ULL, 2ULL, 14ULL, 1ULL,
    19ULL, 1ULL, 1ULL, 1ULL, 1ULL, 19ULL, 1ULL, 14ULL, 2ULL, 4ULL,
    1ULL, 1ULL, 1ULL, 6ULL, 4ULL, 1ULL, 60ULL
};

seq_t a(idx_t n)
{
    seq_t p_nm2 = 0;   /* p_{-2} */
    seq_t p_nm1 = 1;   /* p_{-1} */
    seq_t p = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0)
            a_i = 30;               /* a_0 */
        else
            a_i = period[(i - 1) % 27]; /* periodic part */

        p = a_i * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p;
    }
    return p_nm1; /* p_n */
}
