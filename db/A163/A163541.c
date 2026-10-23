/*
 * A163541  The absolute direction (0=east, 1=south, 2=west, 3=north) taken by
 *          the type I Hilbert's Hamiltonian walk A163359 at the step n.
 * https://oeis.org/A163541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010873(A163538(n) + A163539(n) + abs(A163538(n)) + 3)
 * Range:     u8 n <= 4096, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   hilbert_xy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/hilbert_xy.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */

    idx_t idx = n - 1;
    idx_t x0, y0, x1, y1;

    hilbert_xy(idx,   &x0, &y0);
    hilbert_xy(idx+1, &x1, &y1);

    int dx = (int)x1 - (int)x0;
    int dy = (int)y1 - (int)y0;

    /* Normal direction: 0=E,1=S,2=W,3=N */
    int dir_norm;
    if (dx ==  1 && dy ==  0) dir_norm = 0;
    else if (dx ==  0 && dy == -1) dir_norm = 1;
    else if (dx == -1 && dy ==  0) dir_norm = 2;
    else if (dx ==  0 && dy ==  1) dir_norm = 3;
    else dir_norm = 0;  /* should never happen */

    /* The OEIS direction is rotated 90° counter‑clockwise. */
    seq_t dir = (seq_t)((dir_norm + 1) & 3);
    return dir;
}
