/*
 * A127907  A 5 x 5 magic square read by rows.
 * https://oeis.org/A127907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) = entry in a 5×5 normal magic square read by rows (the
 *            classic Sylvester square). The sequence has 25 terms:
 *            17,24,1,8,15,23,5,7,14,16,4,6,13,20,22,
 *            10,12,19,21,3,11,18,25,2,9
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* the sequence is defined only for 1 ≤ n ≤ 25 */
    if (n == 0 || n > 25) return 0;

    static bool built = false;
    static seq_t sq[5][5] = { {0} };

    if (!built) {
        /* construct the 5×5 magic square using the Siamese method */
        int r = 0, c = 2;          /* start at (0, middle) with 1 */
        for (seq_t v = 1; v <= 25; ++v) {
            sq[r][c] = v;
            int nr = (r + 4) % 5;  /* candidate: move up one row (r-1) */
            int nc = (c + 1) % 5;  /*           move right one column  */
            if (sq[nr][nc] != 0) { /* if occupied, move down from current */
                nr = (r + 1) % 5;
                nc = c;
            }
            r = nr;
            c = nc;
        }
        built = true;
    }

    idx_t idx  = n - 1;
    idx_t row  = idx / 5;
    idx_t col  = idx % 5;
    return sq[(size_t)row][(size_t)col];
}
