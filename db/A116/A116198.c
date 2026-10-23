/*
 * A116198  A 3 x 3 magic square read by rows.
 * https://oeis.org/A116198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   a(n) is the nth entry of all 3×3 magic squares obtained from the
 *            canonical square 705246381 by successive rotations and
 *            reflections. The sequence lists the entries row‑wise: first the
 *            canonical square, then its rotations, then its reflections
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* n < 1 is undefined */
    idx_t idx    = n - 1;
    idx_t square = idx / 9;          /* which square (0 … 7) */
    idx_t pos    = idx % 9;          /* position within the square */

    /* Only eight distinct squares exist; cycle beyond the eighth if desired. */
    square %= 8;

    /* Base magic square (canonical orientation). */
    static const seq_t base[9] = {7,0,5,2,4,6,3,8,1};

    /* Mapping from new index to old index for each transformation.  */
    static const idx_t trans[8][9] = {
        {0,1,2,3,4,5,6,7,8},                 /* identity                     */
        {6,3,0,7,4,1,8,5,2},                 /* rotate 90° clockwise         */
        {8,7,6,5,4,3,2,1,0},                 /* rotate 180°                  */
        {2,5,8,1,4,7,0,3,6},                 /* rotate 270° clockwise         */
        {2,1,0,5,4,3,8,7,6},                 /* horizontal reflection         */
        {6,7,8,3,4,5,0,1,2},                 /* vertical reflection           */
        {0,3,6,1,4,7,2,5,8},                 /* main diagonal reflection      */
        {8,5,2,7,4,1,6,3,0}                  /* anti‑diagonal reflection     */
    };

    return base[ trans[square][pos] ];
}
