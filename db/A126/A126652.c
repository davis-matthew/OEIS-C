/*
 * A126652  A 3 X 3 magic square with magic sum 75: the Loh-Shu square A033812
 *          multiplied by 5.
 * https://oeis.org/A126652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = 5 * lo_shu(n) // lo_shu(n) is the 3×3 normal magic square
 *            (A033812) scaled by 5
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   lo_shu_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/lo_shu_square.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* below offset is undefined */

    idx_t idx0 = n - 1;        /* zero-based index */
    idx_t sq   = (idx0 / 9) % 8;
    idx_t pos  = idx0 % 9;

    int r = pos / 3;
    int c = pos % 3;
    int rr = r, cc = c;

    /* Transform coordinates according to the 8 symmetries of the base square */
    switch (sq) {
    case 0: /* identity */                  rr = r;  cc = c;  break;
    case 1: /* rotate 90° clockwise */     rr = 2 - c; cc = r;  break;
    case 2: /* rotate 180° */               rr = 2 - r; cc = 2 - c; break;
    case 3: /* rotate 270° clockwise */    rr = c;  cc = 2 - r; break;
    case 4: /* reflect vertical */          rr = r;  cc = 2 - c; break;
    case 5: /* reflect horizontal */         rr = 2 - r; cc = c;  break;
    case 6: /* reflect main diagonal */     rr = c;  cc = r;  break;
    case 7: /* reflect anti-diagonal */     rr = 2 - c; cc = 2 - r; break;
    default: break; /* unreachable */
    }

    static seq_t base[9];
    static bool initialized = false;
    if (!initialized) {
        lo_shu_square(base);
        initialized = true;
    }

    /* Multiply by 5 to obtain the required magic sum of 75. */
    seq_t val = base[rr * 3 + cc] * (seq_t)5;
    return val;
}
