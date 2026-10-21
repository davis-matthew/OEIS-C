/*
 * A073092  Number of numbers of the form x^2 + y^2 (0 <= x <= y) less than or
 *          equal to n.
 * https://oeis.org/A073092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of pairs (x,y) with 0 ≤ x ≤ y and x² + y² ≤ n * =
 *            ∑_{k=0..n} A000161(k)
 * Range:     u8 n <= 469, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (A000161(k) counts unordered representations of k)    */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) using binary search.
 * Works for very large n without overflow. */
static inline idx_t int_sqrt(idx_t n)
{
    idx_t l = 0, r = n;
    while (l <= r)
    {
        idx_t m = l + ((r - l) >> 1);
        /* use 128‑bit multiplication to avoid overflow */
        if ((unsigned __int128)m * m <= n)
            l = m + 1;
        else
            r = m - 1;
    }
    return l - 1;
}

/* Count all pairs (x,y) with 0 ≤ x ≤ y and x² + y² ≤ n. */
seq_t a(idx_t n)
{
    seq_t count = 0;

    /* maximum possible x is floor(sqrt(n)) */
    idx_t xmax = int_sqrt(n);

    for (idx_t x = 0; x <= xmax; ++x)
    {
        /* x² fits in seq_t and is <= n, cast to idx_t for subtraction */
        seq_t x2_seq = (seq_t)x * (seq_t)x;
        idx_t x2 = (idx_t)x2_seq;        /* safe since x ≤ sqrt(n) */
        idx_t rem = n - x2;               /* remaining value for y² */
        idx_t y_max = int_sqrt(rem);

        /* y starts at x to enforce x ≤ y */
        if (y_max >= x)
            count += (seq_t)(y_max - x + 1);
    }

    return count;
}
