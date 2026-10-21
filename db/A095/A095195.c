/*
 * A095195  T(n,0) = prime(n), T(n,k) = T(n,k-1)-T(n-1,k-1), 0<=k<n, triangle
 *          read by rows.
 * https://oeis.org/A095195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, look
 * Formula:   a(n) is the (n‑th) entry of the triangle T defined by T(n,0) =
 *            prime(n) T(n,k) = T(n,k-1) - T(n-1,k-1) for 0 ≤ k < n The
 *            sequence a(n) is the triangle read row by row
 * Range:     i8 n <= 54, i16 n <= 170, i32 n <= 629, i64 n <= 2144,
 *            i128 n <= 8384, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* maximum number of rows to pre‑compute, sufficient for n <= 10000 */
#define MAX_ROWS 200

/* static storage for primes (1‑based) and the triangle */
static seq_t primes[MAX_ROWS + 1];
static seq_t triangle[MAX_ROWS + 1][MAX_ROWS + 1];
static bool initialized = false;

/* generate the first MAX_ROWS primes and build the triangle */
static void init(void)
{
    size_t count = 0;
    idx_t cand = 2;                 /* candidate for next prime */
    while (count < MAX_ROWS) {
        bool isprime = true;
        for (idx_t d = 2; d * d <= cand; ++d) {
            if (cand % d == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            ++count;
            primes[count] = (seq_t)cand;
        }
        ++cand;
    }

    /* build the triangle: triangle[r][k] corresponds to T(r,k) (r>=1, 0<=k<r) */
    for (idx_t r = 1; r <= MAX_ROWS; ++r) {
        triangle[r][0] = primes[r];          /* T(r,0) = prime(r) */
        for (idx_t k = 1; k < r; ++k) {
            triangle[r][k] = triangle[r][k - 1] - triangle[r - 1][k - 1];
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init();
    }

    /* determine row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* index within the row (0‑based) */
    idx_t before = (r - 1) * r / 2;
    idx_t k = n - before - 1;

    return triangle[r][k];
}
