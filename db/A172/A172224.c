/*
 * A172224  Number of ways to place 6 nonattacking zebras on a 6 X n board.
 * https://oeis.org/A172224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (1944n^6-27540n^5+227070n^4-1222555n^3+4366071n^2-9580580n+9925860)/30,
 *            n>=15. For n < 15, use the pre‑computed values
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 22, u64 n <= 813, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Precomputed values for n = 1..14 (offset 1). */
    static const seq_t vals[14] = {
        (seq_t)1,          /* a(1) */
        (seq_t)924,        /* a(2) */
        (seq_t)8989,       /* a(3) */
        (seq_t)37270,      /* a(4) */
        (seq_t)145233,     /* a(5) */
        (seq_t)525796,     /* a(6) */
        (seq_t)1605490,    /* a(7) */
        (seq_t)4136952,    /* a(8) */
        (seq_t)9435413,    /* a(9) */
        (seq_t)19632414,   /* a(10) */
        (seq_t)37957424,   /* a(11) */
        (seq_t)69050898,   /* a(12) */
        (seq_t)119351315,  /* a(13) */
        (seq_t)197524064   /* a(14) */
    };

    if (n < 15) {
        return vals[n - 1];
    }

    /* Use 128‑bit arithmetic for safety; the final result fits in seq_t. */
    unsigned __int128 tn = (unsigned __int128)(seq_t)n;
    unsigned __int128 n2 = tn * tn;            /* n^2 */
    unsigned __int128 n3 = n2 * tn;            /* n^3 */
    unsigned __int128 n4 = n3 * tn;            /* n^4 */
    unsigned __int128 n5 = n4 * tn;            /* n^5 */
    unsigned __int128 n6 = n5 * tn;            /* n^6 */

    unsigned __int128 num =
        1944u * n6
      - 27540u * n5
      + 227070u * n4
      - 1222555u * n3
      + 4366071u * n2
      - 9580580u * tn
      + 9925860u;

    seq_t result = (seq_t)(num / 30u);
    return result;
}
