/*
 * A213347  5-quantum transitions in systems of N>=5 spin 1/2 particles, in
 *          columns by combination indices.
 * https://oeis.org/A213347
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) = 2^(N-5-2*k) * C(N,k) * C(N-k,5+k) where N is the row
 *            index (N>=5) and k is the column index for the flattened
 *            triangle A213347
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 69, u64 n <= 265,
 *            u128 n <= 1040, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities */
#include "../utils/pow2.h"
#include "../utils/binomial.h"

/* Return the total number of terms in rows 5 … N (inclusive). */
static inline idx_t cumulative_terms(idx_t N)
{
    if (N < 5) return 0;          /* no rows before 5 */
    idx_t d = N - 5;               /* d >= 0 */
    if ((d & 1) == 0) {           /* d even: d = 2q */
        idx_t q = d >> 1;
        return (q + 1) * (q + 1);
    } else {                       /* d odd: d = 2q+1 */
        idx_t q = d >> 1;
        return q * q + 3 * q + 2;
    }
}

/* Compute a(n) for OEIS sequence A213347.  */
seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* behaviour below offset is irrelevant */

    idx_t z = n - 5;               /* zero‑based position in the flattened triangle */

    /* Binary search for the unique N such that
       cumulative_terms(N-1) <= z < cumulative_terms(N). */
    idx_t lo = 5;
    idx_t hi = n;                  /* cumulative_terms(hi) > z for all n >= 5 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cumulative_terms(mid) > z)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t N = lo;                  /* the row containing the term */

    idx_t prevS = cumulative_terms(N - 1);
    idx_t k = z - prevS;           /* column within row N */

    idx_t exp = (N - 5) - (2 * k); /* exponent for the power of two */
    seq_t val = pow2(exp);
    val = val * binomial(N, k);
    val = val * binomial(N - k, 5 + k);
    return val;
}
