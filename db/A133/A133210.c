/*
 * A133210  Limiting word of the morphism 1->{1, 6, 10, 12}, 2-> {2, 5, 9,
 *          11}, 3-> {1, 3, 8, 12}, 4-> {2, 4, 7, 11}, 5-> {1, 5, 6, 12}, 6->
 *          {2, 6, 5, 11}, 7-> {1, 7, 4, 12}, 8-> {2, 8, 3, 11}, 9-> {1, 9, 2,
 *          12}, 10-> {2, 10, 5, 11}, 11-> {1, 11, 6, 12}, 12-> {2, 12, 7,
 *          11}.
 * https://oeis.org/A133210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This sequence is the limit word of the morphism on indices 1‑12:
      1 → {1,6,10,12}
      2 → {2,5,9,11}
      3 → {1,3,8,12}
      4 → {2,4,7,11}
      5 → {1,5,6,12}
      6 → {2,6,5,11}
      7 → {1,7,4,12}
      8 → {2,8,3,11}
      9 → {1,9,2,12}
     10 → {2,10,5,11}
     11 → {1,11,6,12}
     12 → {2,12,7,11}
   Starting from the initial letter 1, the infinite word is obtained by
   iterating this morphism.  The terms a(n) are the n-th letter (1‑based)
   of that infinite word.

   Algorithm:  n‑1 is expressed in base‑4.  Each base‑4 digit (most
   significant first) selects the next letter via the substitution map.
   This is equivalent to reading the path in the 4‑ary tree defined by
   the morphism.  Complexity is O(log₄ n), trivial for n ≤ 10⁴.
   The algorithm uses only integer arithmetic and static lookup
   tables, fitting the code‑generation limits. */

#include <stdint.h>

static const int trans[13][4] = {
    {0, 0, 0, 0},               /* unused 0‑th row */
    {1, 6,10,12},              /* 1  */
    {2, 5, 9,11},              /* 2  */
    {1, 3, 8,12},              /* 3  */
    {2, 4, 7,11},              /* 4  */
    {1, 5, 6,12},              /* 5  */
    {2, 6, 5,11},              /* 6  */
    {1, 7, 4,12},              /* 7  */
    {2, 8, 3,11},              /* 8  */
    {1, 9, 2,12},              /* 9  */
    {2,10, 5,11},              /*10  */
    {1,11, 6,12},              /*11  */
    {2,12, 7,11}               /*12  */
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n==0, but safe */

    idx_t m = n - 1;               /* 0‑based position */
    /* collect base‑4 digits (least significant first) */
    int digits[64];
    int dcnt = 0;
    do {
        digits[dcnt++] = (int)(m & 3ULL);   /* m % 4 */
        m >>= 2;                            /* m /= 4 */
    } while (m > 0);

    int letter = 1;                 /* start letter */
    for (int i = dcnt - 1; i >= 0; --i) { /* most significant first */
        int digit = digits[i];
        letter = trans[letter][digit];
    }
    return (seq_t)letter;
}
