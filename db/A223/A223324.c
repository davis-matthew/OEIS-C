/*
 * A223324  Rolling icosahedron footprints: number of 4Xn 0..11 arrays
 *          starting with 0 where 0..11 label vertices of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves along an icosahedral edge.
 * https://oeis.org/A223324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 121*a(n-1) - 3029*a(n-2) + 25559*a(n-3) - 89707*a(n-4) +
 *            122263*a(n-5) - 12831*a(n-6) - 60039*a(n-7) for n>10 Initial
 *            values (n = 1..10): a(1)=1728, a(2)=78125, a(3)=6865625,
 *            a(4)=603351125, a(5)=54279694625, a(6)=4903804407125,
 *            a(7)=443803619416625, a(8)=40180564679055125, a(9)=3638107937...
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* Static table of the first ten terms as strings */
    static const char *init_str[10] = {
        "1728",
        "78125",
        "6865625",
        "603351125",
        "54279694625",
        "4903804407125",
        "443803619416625",
        "40180564679055125",
        "3638107937069854625",
        "329414029399378035125"
    };
    static seq_t init[10];
    static int init_done = 0;
    if (!init_done) {
        for (int i = 0; i < 10; ++i)
            init[i] = seq_from_str(init_str[i]);
        init_done = 1;
    }

    if (n <= 10)
        return init[n-1];

    /* Prepare the last seven values needed for the recurrence.
       For the first iteration (n = 11) we need a(10) ... a(4). */
    seq_t prev[7];
    prev[0] = init[9]; /* a(10) */
    prev[1] = init[8]; /* a(9)  */
    prev[2] = init[7]; /* a(8)  */
    prev[3] = init[6]; /* a(7)  */
    prev[4] = init[5]; /* a(6)  */
    prev[5] = init[4]; /* a(5)  */
    prev[6] = init[3]; /* a(4)  */

    for (idx_t k = 11; k <= n; ++k) {
        seq_t val = 121 * prev[0];
        val -= 3029 * prev[1];
        val += 25559 * prev[2];
        val -= 89707 * prev[3];
        val += 122263 * prev[4];
        val -= 12831 * prev[5];
        val -= 60039 * prev[6];

        /* shift the window for the next term */
        for (int i = 6; i >= 1; --i)
            prev[i] = prev[i-1];
        prev[0] = val;
    }

    return prev[0];
}
