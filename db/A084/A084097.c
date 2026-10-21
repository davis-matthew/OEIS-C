/*
 * A084097  Square array whose rows have e.g.f. exp(x)*cosh(sqrt(k)*x), k>=0,
 *          read by ascending antidiagonals.
 * https://oeis.org/A084097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by reading a square array whose rows have the
 *            exponential generating function exp(x) * cosh(sqrt(k) * x) by
 *            ascending antidiagonals. Equivalently, a(n) is the flattened
 *            triangle T(n, k) = A(n-k, k) where A(m, k) = sum_{j=0..⌊k/2⌋}
 *            binomial(k, 2j) * m^j. The function below compute...
 * Range:     u8 n <= 52, u16 n <= 130, u32 n <= 342, u64 n <= 933,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the helper function 'binom' from utils/binom.h. */
#include "../utils/binom.h"

static inline seq_t pow_seq_local(seq_t base, idx_t exp)
{
    /* Multiplicative exponentiation for the (possibly big) seq_t type. */
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative number of elements up to
       row r‑1 is ≤ n < cumulative up to row r.  The cumulative number of
       elements in rows 0 .. r is (r+1)(r+2)/2. */
    idx_t r = 0;
    /* Incrementally find r; r stays ≤ sqrt(2n).  The loop runs at most ~√(2n)
       iterations, which is tiny for n ≤ 10⁴.  The multiplication is done in
       unsigned __int128 to avoid overflow for larger values of n. */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Number of elements before row r (i.e., in rows 0 … r‑1). */
    unsigned __int128 prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - (idx_t)prev;          /* column index within row r */
    idx_t m = r - c;                    /* the 'm' value used in A(m, c) */

    /* Compute A(m, c) = Σ_{j=0..⌊c/2⌋} C(c, 2j) * m^j. */
    seq_t result = 0;
    seq_t power   = 1;                  /* m^j, starts with m^0 = 1 */
    for (idx_t j = 0; j <= c / 2; ++j) {
        seq_t bino = binom(c, 2 * j);   /* C(c, 2j) */
        result += bino * power;
        power *= (seq_t)m;              /* next power of m */
    }
    return result;
}
