/*
 * A130556  A model of the atomic nucleus (Shell model of nucleus). A
 *          triangle.
 * https://oeis.org/A130556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation generates the terms of OEIS A130556.
 *
 *  The sequence is formed by writing a triangle whose rows contain
 *  repunit integers.  For a given index n (1‑based) we first find the
 *  row r such that T(r‑1) < n ≤ T(r) with T(r)=r(r+1)/2.
 *  In row r the integers are arranged according to the following rule:
 *
 *     • If r is even:  even numbers descending (r, r‑2, …, 2) 
 *                      followed by odd numbers ascending (1, 3, …, r‑1).
 *     • If r is odd:   odd numbers descending (r, r‑2, …, 1) 
 *                      followed by even numbers ascending (2, 4, …, r‑1).
 *
 *  The column position c (1‑based) within the row gives the length l
 *  of the repunit to output.  The repunit of length l is
 *      (10^l – 1) / 9   (or equivalently the decimal number consisting of
 *      l occurrences of the digit 1).
 *
 *  The function works for any of the predefined integer types
 *  seq_t and idx_t, including the arbitrary‑precision integer
 *  implementation provided for the C++17 build.  It runs in
 *  O(√n + l) time and uses only integer arithmetic.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that T(r-1) < n <= T(r). */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) r++;

    /* Position of n within its row (1‑based). */
    idx_t col = n - (r - 1) * r / 2;

    /* Determine the length l of the repunit. */
    idx_t l;
    if ((r & 1) == 0) /* even row */
    {
        idx_t half = r / 2;
        if (col <= half)
            l = r - 2 * (col - 1);               /* even descending */
        else
        {
            idx_t cprime = col - half;
            l = 2 * (cprime - 1) + 1;             /* odd ascending   */
        }
    }
    else            /* odd row */
    {
        idx_t half_odd = (r + 1) / 2;            /* ceil(r/2)      */
        if (col <= half_odd)
            l = r - 2 * (col - 1);               /* odd descending  */
        else
        {
            idx_t cprime = col - half_odd;
            l = 2 * cprime;                       /* even ascending  */
        }
    }

    /* Build the repunit of length l. */
    seq_t value = 0;
    for (idx_t i = 0; i < l; ++i)
        value = value * 10 + 1;

    return value;
}
