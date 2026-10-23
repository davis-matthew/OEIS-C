/*
 * A129360  Triangle read by rows: A054525 * A115361 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A129360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 35, i16 n <= 1274, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A129360 (OEIS A129360) is the row‑reading of the triangle
 *
 *  T(r,k) = A087003(r/k)      if k | r
 *          = 0                otherwise
 *
 *  A087003(m) = 0 if m is even,
 *              μ(m) if m is odd,
 *  where μ is the Möbius function.
 *
 *  For a given index n (1‑based) the function finds the row r and
 *  column k with
 *      n = r(r-1)/2 + k,  1 ≤ k ≤ r
 *  and returns the corresponding value of T(r,k).
 */

seq_t a(idx_t n)
{
    /* ---- find row r such that r(r+1)/2 >= n ---- */
    idx_t lo = 1, hi = n;          /* hi is safe upper bound */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        seq_t tri = ((seq_t)mid * (seq_t)(mid + 1)) / 2;
        if (tri >= (seq_t)n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;

    /* ---- column k in that row ---- */
    idx_t prev = (r - 1) * r / 2;  /* number of elements before row r */
    idx_t k = n - prev;            /* 1‑based column index */

    /* No entry if k does not divide r */
    if (r % k != 0)
        return 0;

    /* ----- compute j = r/k and evaluate A087003(j) ---- */
    idx_t j = r / k;

    /* A087003 is zero for even j */
    if ((j & 1) == 0)
        return 0;

    /* Möbius function μ(j) for odd j */
    if (j == 1)
        return 1;

    int prime_cnt = 0;
    idx_t temp = j;

    /* factor only odd primes, j is odd */
    for (idx_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            /* check for a squared factor */
            if ((temp / d) % d == 0)
                return 0;           /* divisible by a square */

            prime_cnt++;
            temp /= d;
            /* after removing one factor, any further division would imply a
               higher power — already ruled out by the square test */
        }
    }

    if (temp > 1)           /* remaining prime factor */
        prime_cnt++;

    return (prime_cnt % 2 == 0) ? 1 : -1;
}
