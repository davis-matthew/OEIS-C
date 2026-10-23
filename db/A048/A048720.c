/*
 * A048720  Multiplication table {0..i} X {0..j} of binary polynomials
 *          (polynomials over GF(2)) interpreted as binary vectors, then
 *          written in base 10; or, binary multiplication without carries.
 * https://oeis.org/A048720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, hear, tabl
 * Formula:   a(n) = i * j (product of binary polynomials over GF(2)) * * Let
 *            T(k) = k*(k+1)/2 (k-th triangular number). * For a given n, find
 *            the unique k such that T(k) ≤ n < T(k+1). * Then i = n - T(k), j
 *            = k - i. * The value a(n) is the binary-polynomial product i *
 *            j, * which is obtained by shifting i lef...
 * Range:     u8 n <= 543, u16 n <= 9900, u32 n <= 9900, u64 n <= 9900,
 *            u128 n <= 9900, big any n
 * Helpers:   gf2_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gf2_mul.h"

/* triangular number: T(k) = k*(k+1)/2, computed with 128‑bit arithmetic
 * to avoid overflow for large k. */
static idx_t
triangular(idx_t k)
{
    return (idx_t)(((__int128)k * (k + 1)) / 2);
}

/* binary search for the greatest k with T(k) ≤ n */
static idx_t
find_k(idx_t n)
{
    idx_t low = 0, high = n;          /* high is safe because T(n) > n for n>0 */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* The required sequence function */
seq_t
a(idx_t n)
{
    idx_t k = find_k(n);                /* diagonal index */
    idx_t tri = triangular(k);          /* number of terms before diagonal k */
    idx_t i = n - tri;                  /* row index in the diagonal */
    idx_t j = k - i;                    /* column index */
    return gf2_mul(i, j);
}
