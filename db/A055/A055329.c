/*
 * A055329  Number of rooted identity trees with n nodes and 4 leaves.
 * https://oeis.org/A055329
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(n==0) + (60*n^6 -1236*n^5 +9450*n^4 -33520*n^3
 *            +59940*n^2 -66294*n +48065)/69120 +(-1)^n*(4*n^2 -38*n +89)/512
 *            +(3/32)*(-1)^floor((n+1)/2) +ChebyshevU(n, -1/2)/27
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 133, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Allowed only header: <stdint.h> for basic integer types */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;               /* behaviour below the offset does not matter */

    /* Use signed 128‑bit intermediate arithmetic */
    __int128 n128 = (unsigned long long)n;

    /* ---- term 1: polynomial over 69120 ---------------------------------- */
    __int128 p = 60;                      /* Horner representation  */
    p = p * n128 - 1236;
    p = p * n128 + 9450;
    p = p * n128 - 33520;
    p = p * n128 + 59940;
    p = p * n128 - 66294;
    p = p * n128 + 48065;                 /* 60*n^6 + … + 48065   */
    __int128 num = p;                     /* current numerator (denominator 69120) */

    /* ---- term 2: (-1)^n*(4*n^2-38*n+89)/512  (factor 135) ----------- */
    __int128 sign1 = (n128 & 1) ? -1 : 1;
    __int128 val1 = 4 * n128 * n128 - 38 * n128 + 89;
    num += sign1 * val1 * 135;            /* 69120/512 = 135 */

    /* ---- term 3: (3/32)*(-1)^floor((n+1)/2)  (factor 2160) ----------- */
    __int128 half = (n128 + 1) >> 1;      /* floor((n+1)/2) */
    __int128 sign3 = (half & 1) ? -1 : 1;
    num += sign3 * 3 * 2160;              /* 69120/32 = 2160 */

    /* ---- term 4: ChebyshevU(n,-1/2)/27  (factor 2560) ---------------- */
    int mod3 = (int)(n128 % 3);           /* 0,1,2 */
    __int128 uval = 0;
    if (mod3 == 0)          uval = 1;
    else if (mod3 == 1)     uval = -1;   /* mod3==2 → 0 */
    num += uval * 2560;                     /* 69120/27 = 2560 */

    /* final result: divide by 69120 */
    seq_t result = (seq_t)(num / 69120);
    return result;
}
