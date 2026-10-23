/*
 * A140874  Triangle T(n,k) = binomial(n,k+2)-2*binomial(n,k+1)-binomial(n,k)
 *          read by rows, 0<=k<=n-2, n>=2.
 * https://oeis.org/A140874
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, tabl, sign
 * Formula:   T(r,k) = binomial(r,k+2) – 2*binomial(r,k+1) – binomial(r,k),
 *            read row by row starting at r = 2 with (r−1) elements. a(n) is
 *            the (n−2)th element in this triangle (n ≥ 2)
 * Range:     i8 n <= 25, i16 n <= 46, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We avoid external helpers by building a small Pascal triangle
   at most up to r ≈ sqrt(2n).  The triangle is rebuilt only once
   and reused for all calls.  This keeps the routine fast enough
   for n up to about 10000. */

#include <stdint.h>

/* Max r needed for n <= 10000: r satisfies r*(r−1)/2 ≥ 9998
   ⇒ r ≈ 142.  We round up to 200 for safety. */
#define MAX_R 200

/* static cache of binomial coefficients up to MAX_R */
static seq_t binom[MAX_R + 1][MAX_R + 1];
static int binom_initialized = 0;

/* Helper to initialise the Pascal triangle lazily */
static void init_binom(void)
{
    if (binom_initialized) return;
    binom[0][0] = 1;
    for (idx_t n = 1; n <= MAX_R; ++n) {
        binom[n][0] = 1;
        binom[n][n] = 1;
        for (idx_t k = 1; k < n; ++k)
            binom[n][k] = binom[n - 1][k - 1] + binom[n - 1][k];
    }
    binom_initialized = 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;           /* outside the offset */

    init_binom();

    /* Position of the element in the triangular reading, 0‑based */
    idx_t i = n - 2;

    /* Find the row r such that i < r*(r-1)/2.
       Small loop is fine because r <= ~150 for n <= 10000. */
    idx_t r = 2;
    while (i >= r * (r - 1) / 2) r++;

    /* Inside this row, column k.  0 <= k <= r-2 */
    idx_t prev = (r - 2) * (r - 1) / 2; /* elements before row r */
    idx_t k = i - prev;

    /* Compute T(r,k) = C(r,k+2) – 2*C(r,k+1) – C(r,k). */
    seq_t val = binom[r][k + 2];
    val -= 2 * binom[r][k + 1];
    val -= binom[r][k];

    return val;
}
