/*
 * A107292  1-limiting word of the morphism 1->1331, 2->313, 3->2.
 * https://oeis.org/A107292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107292 – 1‑limiting word of the morphism
   φ : 1 → 1331 , 2 → 313 , 3 → 2
   (seeking the nth symbol of the fixed point, offset 0). */
#include <stdint.h>

static const unsigned char child[3][4] = {
    {0, 2, 2, 0},         /* 1 → 1331 */
    {2, 0, 2, 0},         /* 2 → 313  */
    {1, 0, 0, 0}          /* 3 → 2    */
};
static const unsigned char childlen[3] = {4, 3, 1};

static const int MAX_DEPTH = 64;   /* sufficient for very large n */

seq_t a(idx_t n)
{
    /* lengths[d][l] – length of φ^d(letter l),
       l = 0 for 1, l = 1 for 2, l = 2 for 3. */
    idx_t lengths[MAX_DEPTH + 1][3];
    for (int d = 0; d <= MAX_DEPTH; ++d)
        for (int l = 0; l < 3; ++l)
            lengths[d][l] = 0;

    lengths[0][0] = lengths[0][1] = lengths[0][2] = 1;

    int depth = 0;
    /* grow the table until φ^depth(1) is longer than n */
    while (lengths[depth][0] <= n && depth < MAX_DEPTH) {
        for (int l = 0; l < 3; ++l) {
            idx_t sum = 0;
            for (int i = 0; i < childlen[l]; ++i) {
                int c = child[l][i];
                sum += lengths[depth][c];
            }
            lengths[depth + 1][l] = sum;
        }
        ++depth;
    }

    /* descend the expansion tree to find the symbol at position n */
    int letter = 0;              /* start from the initial word “1” */
    idx_t pos = n;
    int d = depth;

    while (d > 0) {
        for (int i = 0; i < childlen[letter]; ++i) {
            int c = child[letter][i];
            idx_t clen = lengths[d - 1][c];
            if (pos < clen) {
                letter = c;
                d -= 1;
                break;
            } else {
                pos -= clen;
            }
        }
    }
    return (seq_t)(letter + 1);  /* back to values 1, 2, or 3 */
}
