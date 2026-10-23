/*
 * A023114  Squares that remain square when the digit 9 is appended.
 * https://oeis.org/A023114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Squares of y in solutions to x^2 - 10*y^2 = 9. For each solution
 *            (x,y) the next solution is given by x' = 19*x + 60*y y' = 6*x +
 *            19*y The sequence lists y² sorted in increasing order, starting
 *            with seeds (x,y) = (3,0), (7,2), (13,4)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* local struct for a Pell solution pair */
    typedef struct {
        seq_t x;
        seq_t y;
    } pair_t;

    /* initial seeds for the three chains */
    pair_t chains[3] = {
        { (seq_t)3, (seq_t)0 },   /* (x,y) = (3,0)  -> y = 0  */
        { (seq_t)7, (seq_t)2 },   /* (x,y) = (7,2)  -> y = 2  */
        { (seq_t)13, (seq_t)4 }   /* (x,y) = (13,4) -> y = 4  */
    };

    seq_t result = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        /* choose the chain with the smallest next y value */
        int best = 0;
        for (int j = 1; j < 3; ++j) {
            if (chains[j].y < chains[best].y)
                best = j;
        }

        /* square the selected y to get the sequence value */
        seq_t y = chains[best].y;
        result = y * y;

        /* advance that chain to its next solution */
        seq_t x = chains[best].x;
        seq_t yy = chains[best].y;
        chains[best].x = (seq_t)19 * x + (seq_t)60 * yy;
        chains[best].y = (seq_t)6 * x + (seq_t)19 * yy;
    }

    return result;
}
