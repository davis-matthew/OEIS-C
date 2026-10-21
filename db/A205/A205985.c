/*
 * A205985  Number of (n+1) X 8 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly two clockwise edge increases.
 * https://oeis.org/A205985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 588*a(n-3) for n>12, with a(1)=1643538, a(2)=2482596,
 *            a(3)=6296976, a(4)=35107968, a(5)=279003648, a(6)=2260666464,
 *            a(7)=19410499584, a(8)=160992300384, a(9)=1323215405568,
 *            a(10)=11406249937152, a(11)=94660267282560,
 *            a(12)=778050772950528
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 16, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[13] = {
        0ULL,                     /* unused, 0 index */
        1643538ULL,               /* a(1) */
        2482596ULL,               /* a(2) */
        6296976ULL,               /* a(3) */
        35107968ULL,              /* a(4) */
        279003648ULL,             /* a(5) */
        2260666464ULL,            /* a(6) */
        19410499584ULL,           /* a(7) */
        160992300384ULL,          /* a(8) */
        1323215405568ULL,         /* a(9) */
        11406249937152ULL,        /* a(10) */
        94660267282560ULL,        /* a(11) */
        778050772950528ULL        /* a(12) */
    };

    if (n <= 12) {
        return init[n];
    }

    /* Map n onto its residue class modulo 3 and find the first known
       value in that class (indices 10, 11, 12). */
    idx_t r = n % 3;
    idx_t baseIndex = (r == 1) ? 10 :
                      (r == 2) ? 11 : 12;
    idx_t k = (n - baseIndex) / 3;   /* Number of times we step back by 3. */

    seq_t result = init[baseIndex];
    while (k--) {
        result *= 588;
    }
    return result;
}
