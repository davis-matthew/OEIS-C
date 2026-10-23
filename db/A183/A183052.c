/*
 * A183052  Sums of knight's moves from (0,0) to points on the square
 *          |i|+|j|=n on infinite chessboard.
 * https://oeis.org/A183052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{⌊n/2⌋} w(i,n-i) * knight_dist(i,n-i) where
 *            w(a,b) = 1 if a=b=0, 4 if a=0 or b=0 or a=b>0, 8 otherwise.
 *            knight_dist(x,y) is the minimal number of knight moves from
 *            (0,0) to (x,y) on the infinite chessboard
 * Range:     u8 n <= 12, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t knight_dist(idx_t x, idx_t y)
{
    /* ensure x >= y */
    if (x < y) {
        idx_t t = x; x = y; y = t;
    }

    if (x == 0 && y == 0) return (seq_t)0;
    if (x == 1 && y == 0) return (seq_t)3;
    if (x == 2 && y == 2) return (seq_t)4;

    seq_t d1 = (seq_t)((x + (idx_t)1) / (idx_t)2);
    seq_t d2 = (seq_t)((x + y + (idx_t)2) / (idx_t)3);
    seq_t d  = d1 > d2 ? d1 : d2;

    /* parity correction */
    if ( ((d & (seq_t)1) != ((x + y) & (idx_t)1)) )
        d = d + (seq_t)1;

    return d;
}

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    idx_t limit = n >> 1;   /* floor(n/2) */

    for (idx_t a = (idx_t)0; a <= limit; ++a) {
        idx_t b = n - a;
        seq_t w;

        if (a == (idx_t)0 && b == (idx_t)0)
            w = (seq_t)1;
        else if (a == b)
            w = (seq_t)4;
        else if (a == (idx_t)0 || b == (idx_t)0)
            w = (seq_t)4;
        else
            w = (seq_t)8;

        seq_t d = knight_dist(a, b);
        sum = sum + d * w;
    }

    return sum;
}
