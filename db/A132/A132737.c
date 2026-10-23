/*
 * A132737  Triangle T(n,k) = 2*binomial(n,k) + 1, read by rows.
 * https://oeis.org/A132737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term in the row‑wise list of the infinite
 *            triangular array T(n,k) = 1 when k=0 or k=n, otherwise T(n,k) =
 *            2*binomial(n,k)+1
 * Range:     u8 n <= 58, u16 n <= 178, u32 n <= 610, u64 n <= 2307,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Euclidean gcd for unsigned 64‑bit integers. */
static idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* Increase high until it surely exceeds the needed row. */
    while ((((__int128)high) * (high + 1)) / 2 <= n)
        high <<= 1;
    /* Binary search for the largest r with triangular number ≤ n. */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = ((__int128)mid * (mid + 1)) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Compute binomial(n,k) using cancellation to avoid overflow. */
static seq_t binom_reduced(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    if (k == 0) return (seq_t)1;

    /* Allocate arrays for numerator and denominator factors. */
    idx_t *num = (idx_t*)malloc(k * sizeof(idx_t));
    idx_t *den = (idx_t*)malloc(k * sizeof(idx_t));
    for (idx_t i = 0; i < k; ++i)
    {
        num[i] = n - k + 1 + i;   /* values n-k+1, n-k+2, ..., n */
        den[i] = i + 1;           /* values 1, 2, ..., k   */
    }

    /* Cancel common factors between numerators and denominators. */
    for (idx_t i = 0; i < k; ++i)
    {
        if (den[i] == 1) continue;
        for (idx_t j = 0; j < k && den[i] > 1; ++j)
        {
            idx_t g = gcd_u64(num[j], den[i]);
            if (g > 1)
            {
                num[j] /= g;
                den[i] /= g;
            }
        }
        /* den[i] should now be 1; if not it means something went wrong,
           but the binomial is guaranteed to be an integer. */
    }

    /* Multiply the remaining numerators to get the binomial value. */
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        result = result * (seq_t)num[i];

    free(num);
    free(den);
    return result;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    idx_t row = find_row(n);
    idx_t col = n - ((idx_t)(((idx_t)row * (row + 1)) / 2));

    if (col == 0 || col == row)
        return (seq_t)1;

    seq_t binom = binom_reduced(row, col);
    return binom * ((seq_t)2) + (seq_t)1;
}
