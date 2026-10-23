/*
 * A038233  Triangle whose (i,j)-th entry is binomial(i,j)*4^(i-j)*3^j.
 * https://oeis.org/A038233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) is the n-th (row‑major) entry of the triangle T(i,j) =
 *            C(i,j) * 4^(i‑j) * 3^j (i ≥ 0, 0 ≤ j ≤ i)
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------
   Helper: greatest common divisor for seq_t (modulo operator is
   guaranteed to work on seq_t).  It is used to keep intermediate
   binomial numerator/denominator small enough for every seq_t size.
   ------------------------------------------------------------------ */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* ------------------------------------------------------------------
   Helper: binomial coefficient C(n,k) as an exact seq_t value.
   Uses multiplicative algorithm with gcd simplification to avoid
   overflow for small seq_t types (e.g. uint8_t).
   ------------------------------------------------------------------ */
static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;   /* use symmetry */
    seq_t res = 1;
    for (idx_t t = 1; t <= k; ++t)
    {
        seq_t num = (seq_t)(n - k + t);
        seq_t den = (seq_t)t;
        seq_t g1   = seq_gcd(res, den);
        res /= g1;
        den /= g1;
        seq_t g2   = seq_gcd(num, den);
        num /= g2;
        den /= g2;   /* den must be 1 now */
        res *= num;
    }
    return res;
}

/* ------------------------------------------------------------------
   Helper: integer exponentiation (base^exp).  Uses exponentiation by
   squaring; multiplication and division are on seq_t values.
   ------------------------------------------------------------------ */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ------------------------------------------------------------------
   Main sequence function.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Step 1: find the row i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2  */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) i++;
    idx_t tri = i * (i + 1) / 2;   /* number of elements before row i */
    idx_t j = n - tri;            /* column index within the row */

    /* Step 2: compute the entry */
    seq_t term = binomial(i, j);
    term *= pow_seq((seq_t)4, i - j);
    term *= pow_seq((seq_t)3, j);
    return term;
}
