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
 * Validated: standard, pass, 118 terms through n = 4011 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void hilbert_xy(idx_t d, idx_t *x, idx_t *y)
{
    idx_t t = d;
    idx_t rx, ry;
    idx_t s = 1;

    *x = *y = 0;

    /* Process 32 levels (handles up to 64‑bit indices). */
    for (s = 1; s < (1ULL << 32); s <<= 1) {
        /* Extract two bits from t */
        rx = (t >> 1) & 1;
        ry = (t ^ rx) & 1;

        /* Rotation (s is the side length of the current sub‑square) */
        if (ry == 0) {
            if (rx == 1) {
                *x = s - 1 - *x;
                *y = s - 1 - *y;
            }
            /* swap x and y */
            idx_t tmp = *x;
            *x = *y;
            *y = tmp;
        }

        /* Move to the next sub‑square */
        *x += s * rx;
        *y += s * ry;

        t >>= 2;  /* Move to the next pair of bits */
    }
}

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
