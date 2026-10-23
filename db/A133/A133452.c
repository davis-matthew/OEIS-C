/*
 * A133452  3-Limiting word of the morphism 1->{3, 5, 7, 10}; 2->{4, 6, 8,
 *          11}; 3->{1, 5, 7, 10}; 4->{2, 6, 8, 9}; 5->{1, 3, 10, 12}; 6->{2,
 *          4, 9, 11}; 7->{1, 3, 10, 12}; 8->{2, 4, 9, 11}; 9->{4, 6, 8, 11};
 *          10->{1, 5, 7, 12}; 11->{2, 6, 8, 9}; 12->{3, 5, 7, 10}.
 * https://oeis.org/A133452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = value obtained by starting with symbol 1 and, * for each
 *            digit of n−1 in base 4 (most significant first), applying * the
 *            morphism table M. The result is the term a(n)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Morphism table: rows 1‑12, columns 0‑3. */
    static const idx_t M[12][4] = {
        {3,5,7,10},  /* 1 */
        {4,6,8,11},  /* 2 */
        {1,5,7,10},  /* 3 */
        {2,6,8, 9},  /* 4 */
        {1,3,10,12}, /* 5 */
        {2,4, 9,11}, /* 6 */
        {1,3,10,12}, /* 7 */
        {2,4, 9,11}, /* 8 */
        {4,6, 8,11}, /* 9 */
        {1,5, 7,12}, /*10 */
        {2,6, 8, 9}, /*11 */
        {3,5, 7,10}  /*12 */
    };

    if (n == 0)
        return 0;  /* undefined for OEIS; guard against it */

    /* Interpret n‑1 in base‑4. */
    idx_t val = n - 1;
    idx_t p   = 1;
    while (p <= val / 4)
        p *= 4;

    /* Start at symbol 1. */
    idx_t symbol = 1;

    while (p > 0)
    {
        idx_t digit = val / p;   /* current base‑4 digit (0 – 3) */
        val %= p;
        p   /= 4;
        symbol = M[symbol - 1][digit];
    }

    return (seq_t)symbol;
}
