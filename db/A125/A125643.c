/*
 * A125643  Squares and cubes (with repetition).
 * https://oeis.org/A125643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth element (starting at 1) of the sorted multiset *
 *            consisting of all perfect squares and all perfect cubes, * with
 *            sixth powers counted twice
 * Range:     u8 n <= 22, u16 n <= 296, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* i indexes the square sequence i^2,  j indexes the cube sequence j^3   */
    idx_t i = 0, j = 0;
    seq_t sq   = (seq_t)0;          /* current square value i^2            */
    seq_t cube = (seq_t)0;          /* current cube value j^3              */
    seq_t val  = (seq_t)0;          /* value to return for the n-th entry  */
    idx_t count = 0;                /* how many terms have been emitted    */

    while (count < n) {
        if (sq < cube) {
            val = sq;              /* take the next square */
            ++i;
            sq = (seq_t)i * (seq_t)i;
        } else if (cube < sq) {
            val = cube;            /* take the next cube */
            ++j;
            cube = (seq_t)j * (seq_t)j * (seq_t)j;
        } else {                   /* sq == cube – a sixth power.  Emit one,
                                     * increment the square side first,
                                     * leave the cube for the next step */
            val = sq;
            ++i;
            sq = (seq_t)i * (seq_t)i;
            /* cube remains unchanged for the duplicate */
        }
        ++count;
    }
    return val;
}
