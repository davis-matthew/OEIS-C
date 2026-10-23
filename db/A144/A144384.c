/*
 * A144384  T(1,k) = 1 and T(n,k) = [t^k] (1 - t)/(1 - t^n) for n >= 2, square
 *          array read by ascending antidiagonals (n >= 1, k >= 0).
 * https://oeis.org/A144384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   not stated
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A144384
 *
 * The sequence is defined by the square array T(m,k):
 *   T(1,k)   = 1  (k ≥ 0)
 *   T(m,k)   = [t^k] (1 - t) / (1 - t^m)   (m ≥ 2, k ≥ 0)
 *   which is
 *     1  if k mod m == 0
 *     -1 if k mod m == 1
 *      0 otherwise
 *
 * The sequence a(n) is the list of T(m,k) read by successive
 * antidiagonals: for each s = 0,1,2,… the pairs
 *     (m,k) = (s+1,0), (s,1), … , (1,s)
 * are output in that order.
 *
 * The function below maps a 1‑based index n to the corresponding
 * pair (m,k) and returns the appropriate coefficient.
 */

#include <stdint.h>

/* Helper: triangular number (s+1)(s+2)/2 as unsigned __int128 --> idx_t */
static uint64_t triangular(unsigned long long s)
{
    return (uint64_t)((__int128)(s + 1) * (s + 2) / 2);
}

/* Main routine */
seq_t a(idx_t n)
{
    if (n == 1)   /* first element (1,0) */
        return (seq_t)1;

    /* Binary search for smallest s such that triangular(s) >= n */
    unsigned long long low = 0;
    unsigned long long high = 1;
    while (triangular(high) < n)
        high <<= 1;          /* double until enough */

    while (low + 1 < high) {
        unsigned long long mid = (low + high) >> 1;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid;
    }
    unsigned long long s = high;          /* s is the diagonal index */

    uint64_t prevTri = triangular(s - 1);   /* triangular(s-1) = s*(s+1)/2 */
    unsigned long long offset = (unsigned long long)n - prevTri - 1; /* 0‑based offset on diagonal */
    unsigned long long m = s + 1 - offset;      /* n1 in the explanation */
    unsigned long long k = offset;

    /* Compute coefficient */
    if (m == 1)
        return (seq_t)1;

    unsigned long long r = k % m;
    if (r == 0)
        return (seq_t)1;
    else if (r == 1)
        return (seq_t)(-1);
    else
        return (seq_t)0;
}
