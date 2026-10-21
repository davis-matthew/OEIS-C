/*
 * A078802  Triangular array T given by T(n,k) = number of 01-words of length
 *          n containing k 1's, no three of which are consecutive.
 * https://oeis.org/A078802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = T(r,k) where r is the unique row index such that *
 *            r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n - r(r+1)/2. * T(n,k)
 *            counts the binary words of length n with exactly k ones * and
 *            without the pattern “111”. * * Recurrence (for n ≥ 4): * T(n,k)
 *            = T(n-1,k) + T(n-2,k-1) + T(n-3,k-2) * Initial rows:...
 * Range:     u8 n <= 69, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static storage for the computed part of the triangular array. */
static seq_t **triangle = NULL;
static idx_t triangle_size = 0;

/* Build the triangle rows 0 … need (inclusive). */
static void build_triangle(idx_t need)
{
    if (need < triangle_size)
        return;

    triangle = (seq_t **)realloc(triangle, (need + 1) * sizeof(seq_t *));
    for (idx_t i = triangle_size; i <= need; ++i) {
        triangle[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
    }

    for (idx_t i = 0; i <= need; ++i) {
        if (i == 0) {
            triangle[0][0] = 1;
        } else if (i == 1) {
            triangle[1][0] = 1;
            triangle[1][1] = 1;
        } else if (i == 2) {
            triangle[2][0] = 1;
            triangle[2][1] = 2;
            triangle[2][2] = 1;
        } else if (i == 3) {
            triangle[3][0] = 1;
            triangle[3][1] = 3;
            triangle[3][2] = 3;
            triangle[3][3] = 0;
        } else {                     /* n ≥ 4 : use the recurrence */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t val = 0;
                if (k <= i - 1)
                    val += triangle[i - 1][k];
                if (k >= 1 && k - 1 <= i - 2)
                    val += triangle[i - 2][k - 1];
                if (k >= 2 && k - 2 <= i - 3)
                    val += triangle[i - 3][k - 2];
                triangle[i][k] = val;
            }
        }
    }

    triangle_size = need + 1;
}

/* Find row r such that r(r+1)/2 ≤ n.  Uses a small loop; r is at most
 * about sqrt(2n), so the loop is negligible even for large n. */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while (1) {
        __uint128_t next = (__uint128_t)(r + 1) * (r + 2) / 2;
        if (next > n)
            break;
        ++r;
    }
    return r;
}

/* Public sequence routine. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    __uint128_t base = (__uint128_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)base;

    build_triangle(r);

    return triangle[r][k];
}
