/*
 * A129321  Triangle where the n-th row is the smallest n positive integers
 *          which are coprime to the n-th triangular number (n(n+1)/2).
 * https://oeis.org/A129321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2839, u16 n <= 9595, u32 n <= 9595, u64 n <= 9595,
 *            u128 n <= 9595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A129321: a(n) = the n-th element of the triangle read row‑major,
   where row r contains the r smallest positive integers coprime to the r-th
   triangular number T_r = r(r+1)/2. */

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* find row r such that r(r+1)/2 >= n */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    idx_t prev_tri = (r - 1) * r / 2;   /* elements before row r */
    idx_t k = n - prev_tri;             /* column index within row r (1‑based) */

    idx_t T = r * (r + 1) / 2;          /* r‑th triangular number */

    idx_t count = 0;
    idx_t x = 1;
    seq_t result = 0;

    while (count < k) {
        if (gcd_idx(x, T) == 1) {
            ++count;
            if (count == k) {
                result = (seq_t)x;
                break;
            }
        }
        ++x;
    }

    return result;
}
