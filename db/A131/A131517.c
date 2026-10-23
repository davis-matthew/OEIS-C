/*
 * A131517  1-limiting word of the morphism 1-> {1, 4, 8, 11}, 2-> {2, 4, 7,
 *          11}, 3-> {3, 6, 10, 11}, 4->{4, 8, 11, 1}, 5-> {5, 6, 10, 1}, 6->
 *          {6, 10, 1, 5}, 7-> {7, 11, 12, 4}, 8->{8, 11, 1, 4}, 9-> {9, 1, 4,
 *          8}, 10-> {10, 11, 3, 6}, 11->{11, 3, 6, 10}, 12->{12, 4, 7, 11}.
 * https://oeis.org/A131517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) is obtained by iterating the given morphism. * Starting
 *            from symbol 1, each symbol expands to four successors. * a(n) is
 *            the symbol at position n in the infinite word, where the * path
 *            to that position is given by the base‑4 digits of (n-1)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint8_t MAP[12][4] = {
    {1, 4, 8, 11}, /* 1 → {1,4,8,11} */
    {2, 4, 7, 11}, /* 2 → {2,4,7,11} */
    {3, 6,10, 11}, /* 3 → {3,6,10,11} */
    {4, 8,11,  1}, /* 4 → {4,8,11,1} */
    {5, 6,10,  1}, /* 5 → {5,6,10,1} */
    {6,10, 1,  5}, /* 6 → {6,10,1,5} */
    {7,11,12,  4}, /* 7 → {7,11,12,4} */
    {8,11, 1,  4}, /* 8 → {8,11,1,4} */
    {9, 1, 4,  8}, /* 9 → {9,1,4,8} */
   {10,11, 3,  6}, /*10 → {10,11,3,6} */
   {11, 3, 6, 10}, /*11 → {11,3,6,10} */
   {12, 4, 7, 11}  /*12 → {12,4,7,11} */
};

seq_t a(idx_t n) {
    /* a(1) = 1, a(n) for n >= 1.  Behavior for n == 0 is undefined. */
    if (n == 0)
        return 0;

    idx_t m = n - 1;
    int digits[64] = {0};
    int len = 0;
    do {
        digits[len++] = (int)(m & 3u);  /* m % 4 */
        m >>= 2u;                      /* m / 4 */
    } while (m != 0);

    uint8_t symbol = 1;  /* start symbol index (1‑based) */
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];                     /* 0 .. 3 */
        symbol = MAP[symbol - 1][d];
    }

    return (seq_t)symbol;
}
