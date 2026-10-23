/*
 * A118729  Rectangular array where row r contains the 8 numbers 4*r^2 - 3*r,
 *          4*r^2 - 2*r, ..., 4*r^2 + 4*r.
 * https://oeis.org/A118729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 4*⌊n/8⌋^2 + ⌊n/8⌋*((n mod 8) – 3)
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n / 8;                     /* row number */
    uint8_t p = n % 8;                   /* position within the row (0..7) */
    seq_t rs = (seq_t)r;                 /* r as seq_t for arithmetic */
    seq_t base = 4 * rs * rs;            /* 4*r^2 */

    switch (p) {
        case 0: return base - 3 * rs;    /* 4r^2 – 3r */
        case 1: return base - 2 * rs;    /* 4r^2 – 2r */
        case 2: return base - rs;       /* 4r^2 –  r */
        case 3: return base;             /* 4r^2      */
        case 4: return base + rs;       /* 4r^2 +  r */
        case 5: return base + 2 * rs;   /* 4r^2 + 2r */
        case 6: return base + 3 * rs;   /* 4r^2 + 3r */
        case 7: return base + 4 * rs;   /* 4r^2 + 4r */
    }
    return base;   /* unreachable, keeps compiler happy */
}
