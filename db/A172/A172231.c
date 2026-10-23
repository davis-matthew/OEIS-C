/*
 * A172231  Number of ways to place 5 nonattacking wazirs on a 5 X n board.
 * https://oeis.org/A172231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (625*n^5-5750*n^4+23535*n^3-54202*n^2+70640*n-41616)/24,
 *            n>=4 with a(1)=0, a(2)=2, a(3)=174
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 45, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)174;

    /* Use 128‑bit arithmetic for the intermediate polynomial evaluation
       to avoid overflow for all 64‑bit inputs. */
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 N2 = N * N;
    unsigned __int128 N3 = N2 * N;
    unsigned __int128 N4 = N3 * N;
    unsigned __int128 N5 = N4 * N;

    unsigned __int128 val =
        (unsigned __int128)625 * N5
        - (unsigned __int128)5750 * N4
        + (unsigned __int128)23535 * N3
        - (unsigned __int128)54202 * N2
        + (unsigned __int128)70640 * N
        - (unsigned __int128)41616;

    val /= 24;
    return (seq_t)val;
}
