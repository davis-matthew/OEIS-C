/*
 * A023856  a(n) = 1*(n+1-1) + 2*(n+1-2) + ... + k*(n+1-k), where k =
 *          floor((n+1)/2).
 * https://oeis.org/A023856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*(4*n^2 + 13*n + 6 - 3*(n+2)*(-1)^n)/48
 * Range:     u8 n <= 12, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for indices below offset */

    /* use 128‑bit arithmetic for safety with small seq_t types */
    unsigned __int128 N = n;                       /* sequence index          */
    unsigned __int128 A = N + 2;                   /* n + 2                   */
    unsigned __int128 B = 4 * N * N + 13 * N + 6;   /* 4n^2 + 13n + 6          */

    if ((n & 1ULL) == 0ULL)          /* n even: subtract 3*A      */
        B -= 3 * A;
    else                            /* n odd:   add 3*A          */
        B += 3 * A;

    unsigned __int128 product = A * B; /* (n+2)*(...)            */
    unsigned __int128 div = product / 48; /* division exact          */
    return (seq_t)div;                 /* cast to the build type   */
}
