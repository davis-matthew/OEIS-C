/*
 * A190361  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=sqrt(5/4), t=sqrt(4/5).
 * https://oeis.org/A190361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*sqrt(5/4)⌋ + ⌊n*sqrt(4/5)⌋ The implementation uses
 *            integer arithmetic only: for seq_t types up to 64‑bit it relies
 *            on 128‑bit intermediates; for larger arbitrary precision seq_t
 *            it uses the provided floor_sqrt utility
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_quotient, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/big_quotient.h"

seq_t a(idx_t n)
{
    /* 0th term is not defined in OEIS; return 0 for convenience. */
    if (n == 0)
        return (seq_t)0;

    /* Fast path for seq_t up to 64 bits using 128‑bit intermediates */
    if (sizeof(seq_t) <= 8U) {
        unsigned __int128 N  = (unsigned __int128)n;
        unsigned __int128 target = (unsigned __int128)5 * N * N;   /* 5*n^2  */

        /* Find K = floor( sqrt(5*n^2) ) by binary search on [0,3*n] */
        unsigned __int128 low  = 0;
        unsigned __int128 high = (unsigned __int128)3 * N + 2;      /* exclusive */
        while (low + 1 < high) {
            unsigned __int128 mid   = (low + high) >> 1;
            unsigned __int128 midSq = mid * mid;
            if (midSq <= target)
                low = mid;
            else
                high = mid;
        }
        unsigned __int128 K = low;                       /* floor(n*sqrt(5))   */

        unsigned __int128 diff = target - K * K;          /* small remainder    */

        unsigned __int128 M = 2 * K + (diff >= K + 1 ? 1 : 0); /* floor(2n*sqrt(5)) */

        seq_t result = (seq_t)n
                     + (seq_t)(K / 2)          /* floor(n*sqrt(5)/2) */
                     + (seq_t)(M / 5);         /* floor(2n/sqrt(5))  */
        return result;
    }

    /* Arbitrary‑precision path using floor_sqrt for seq_t */
    {
        seq_t nn      = (seq_t)n;
        seq_t sq      = nn * nn;          /* n^2                 */
        seq_t five    = (seq_t)5;
        seq_t target  = five * sq;        /* 5*n^2               */

        seq_t K        = floor_sqrt(target);  /* floor(n*sqrt(5))   */
        seq_t diff     = target - K * K;

        seq_t one      = (seq_t)1;
        seq_t two      = (seq_t)2;
        seq_t five_s   = (seq_t)5;
        seq_t zero     = (seq_t)0;

        seq_t M = two * K + (diff >= K + one ? one : zero);

        seq_t floor1 = K / (seq_t)2;         /* floor(n*sqrt(5)/2) */
        seq_t floor2 = M / five_s;           /* floor(2n/sqrt(5))  */

        return nn + floor1 + floor2;
    }
}
