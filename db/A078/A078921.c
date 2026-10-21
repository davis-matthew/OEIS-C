/*
 * A078921  Signed variant of A077012.
 * https://oeis.org/A078921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 28, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(r,k)
 *           r = smallest integer such that r*(r+1)/2 >= n
 *           k = n - r*(r-1)/2
 *           T(r,k) = -(-1)^(r-k+1) * (r/(r-k+1)) * (r-1)!
 *
 * The sequence is the row‑major enumeration of the triangle
 * defined by the formula above.
 */
#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n. */
    double dn = (double)n;
    idx_t r = (idx_t)((sqrt(1.0 + 8.0*dn) - 1.0) / 2.0);
    if (r * (r + 1) / 2 < n) r++;
    /* k is the column index in the row (1‑based). */
    idx_t k = n - r * (r - 1) / 2;

    /* Pre‑compute factorials up to (r-1)! as needed. */
    /* Maximum r for n <= 10000 is 141, but we allow a bit more. */
    enum { MAX_FACT = 200 };
    static seq_t fact[MAX_FACT + 1];
    static int fact_used = 0;   /* indices computed so far (fact[0] always available) */

    if (fact_used == 0) {
        fact[0] = (seq_t)1;
        fact_used = 0;
    }
    /* Compute factorials up to (r-1) if needed */
    if (r - 1 > (idx_t)fact_used) {
        int start = fact_used + 1;
        int end   = (int)(r - 1);
        for (int i = start; i <= end; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_used = end;
    }

    /* Compute the term value. */
    seq_t numer = (seq_t)r * fact[r - 1];
    idx_t denom = r - k + 1;
    seq_t val = numer / (seq_t)denom;

    /* Sign factor: +1 if (r-k+1) is odd, else -1. */
    seq_t sign = ((denom & 1ULL) ? (seq_t)1 : (seq_t)-1);
    return sign * val;
}
