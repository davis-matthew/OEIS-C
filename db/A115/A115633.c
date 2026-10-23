/*
 * A115633  A divide and conquer-related triangle: see formula for T(n,k), n
 *          >= k >= 0.
 * https://oeis.org/A115633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) is the n‑th entry of the left‑justified triangle T with *
 *            T(r,k) = (-1)^r if r == k; * -4 if r == 2*k + 2; * (r % 2) if r
 *            == k + 1; * 0 otherwise * where r and c are determined from n by
 *            * r = ⌊ (√(8n + 1) - 1)/2 ⌋ * c = n - r(r + 1)/2
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Compute the row index r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
static idx_t triangle_row(idx_t n)
{
    /* Initial approximation using floating point */
    long double val = 8.0L * (long double)n + 1.0L;
    idx_t r = (idx_t)floorl((sqrtl(val) - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one errors */
    while (( (__int128)r * (r + 1) / 2 ) > n)
        r--;
    while (( (__int128)(r + 1) * (r + 2) / 2 ) <= n)
        r++;
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = triangle_row(n);

    /* start index of row r */
    __int128 start128 = (__int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;

    idx_t k = n - start;

    /* Compute T(r,k) according to the rules */
    if (r == k) {                /* diagonal */
        return (seq_t)((r & 1) ? -1 : 1);
    }
    else if (r == 2 * k + 2) {   /* -4 entries */
        return (seq_t)-4;
    }
    else if (r == k + 1) {       /* (r % 2) entries */
        return (seq_t)(r % 2);
    }
    else {
        return (seq_t)0;
    }
}
