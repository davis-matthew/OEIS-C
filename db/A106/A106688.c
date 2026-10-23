/*
 * A106688  2-limiting word of the morphism 1->2, 2->34, 3->24, 4->256, 5->36,
 *          6->145.
 * https://oeis.org/A106688
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

/* A106688: 2‑limiting word of the morphism
     1 → 2
     2 → 3 4
     3 → 2 4
     4 → 2 5 6
     5 → 3 6
     6 → 1 4 5

   Computes the n‑th term (0‑based) of the infinite word produced by
   iterating this morphism starting from the word [1]. The algorithm
   first finds a depth D such that the length of φ^D(1) exceeds n,
   then walks the substitution tree from depth D down to depth 0 to
   locate the leaf symbol that occupies position n.  */

/*---------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>

/* mapping data --------------------------------------------------*/
static const int childCount[7] = {0, 1, 2, 2, 3, 2, 3};
static const int child[7][3] = {
    {0, 0, 0},          /* 0 unused */
    {2, 0, 0},          /* 1 → 2 */
    {3, 4, 0},          /* 2 → 3 4 */
    {2, 4, 0},          /* 3 → 2 4 */
    {2, 5, 6},          /* 4 → 2 5 6 */
    {3, 6, 0},          /* 5 → 3 6 */
    {1, 4, 5}           /* 6 → 1 4 5 */
};

/* storage for pre‑computed length rows --------------------------*/
static uint64_t **lenRows = NULL;   /* array of pointers to rows */
static size_t   rowsCap = 0;        /* current malloc capacity */
static size_t   rowsSize = 0;       /* number of rows actually stored */

/* ensure that the length row with index idx exists ----------------*/
static void ensureRow(size_t idx)
{
    if (idx >= rowsSize) {
        /* grow the array of row pointers if necessary */
        if (idx >= rowsCap) {
            size_t newCap = rowsCap ? rowsCap * 2 : 8;
            uint64_t **tmp = (uint64_t **)realloc(lenRows, newCap * sizeof(uint64_t*));
            lenRows = tmp;
            rowsCap = newCap;
        }
        /* allocate the new row */
        lenRows[idx] = (uint64_t *)malloc(7 * sizeof(uint64_t));

        if (idx == 0) {
            /* depth 0: each symbol has length 1 (itself) */
            for (int s = 1; s <= 6; ++s)
                lenRows[0][s] = 1;
        } else {
            /* compute lengths from the previous row */
            for (int s = 1; s <= 6; ++s) {
                uint64_t sum = 0;
                for (int k = 0; k < childCount[s]; ++k)
                    sum += lenRows[idx - 1][child[s][k]];
                lenRows[idx][s] = sum;
            }
        }
        rowsSize = idx + 1;
    }
}

/*--------------- main sequence routine ---------------------------*/
seq_t a(idx_t n)
{
    /* make sure the first two rows exist (depths 0 and 1) */
    if (rowsSize == 0)
        ensureRow(0);
    if (rowsSize < 2)
        ensureRow(1);

    /* find the smallest depth D with lenRows[D][1] > n   */
    size_t depth = 1;
    while (lenRows[depth][1] <= (uint64_t)n) {
        depth++;
        ensureRow(depth);
    }

    /* walk the tree downwards to locate leaf symbol */
    uint64_t pos = (uint64_t)n;
    int symbol = 1;  /* start at the initial symbol */
    for (int d = (int)depth; d > 0; --d) {
        for (int k = 0; k < childCount[symbol]; ++k) {
            int ch  = child[symbol][k];
            uint64_t part = lenRows[d - 1][ch];
            if (pos < part) {
                symbol = ch;
                break;
            }
            pos -= part;
        }
    }

    return (seq_t)symbol;   /* cast to seq_t for arbitrary-precision builds */
}
