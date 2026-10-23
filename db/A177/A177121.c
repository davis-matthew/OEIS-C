/*
 * A177121  Square array T(n,k) read by antidiagonals up: T(n,k) = 1 if n=1;
 *          otherwise if n divides k then T(n,k) = -n+1; otherwise T(n,k) = 1.
 * https://oeis.org/A177121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, look
 * Formula:   a(n) is obtained by reading the square array T(n,k) defined by
 *            T(1,k) = 1, T(n,k) = -n+1 if n divides k, 1 otherwise, by
 *            antidiagonals “up” (diagonal d contains entries (d,1),(d‑1,2),…,
 *            (1,d)). This implementation converts the index n to the
 *            corresponding (i,j) and returns the appropriate value
 * Range:     i8 n <= 9865, i16 n <= 9865, i32 n <= 9865, i64 n <= 9865,
 *            i128 n <= 9865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; n==0 is unused */

    /* Find the diagonal d such that d*(d+1)/2 >= n. */
    idx_t d = 1;
    __int128 total = 1;                /* cumulative count up to diagonal d */
    while (total < n) {
        d++;
        total += d;
    }

    /* Number of entries before this diagonal. */
    idx_t prev = (idx_t)((__int128)(d - 1) * d / 2);

    /* Position (0‑based) within diagonal. */
    idx_t p = n - prev - 1;

    /* Coordinates (i,j) for this position (reading up). */
    idx_t i = d - p;
    idx_t j = p + 1;

    /* Evaluate T(i,j). */
    if (i == 1) return (seq_t)1;
    if (j % i == 0) return (seq_t)(-(seq_t)i + 1);
    return (seq_t)1;
}
