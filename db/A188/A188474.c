/*
 * A188474  A generalized Deutsch triangle.
 * https://oeis.org/A188474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A188474 – Generalized Deutsch triangle (r = 2)
 *
 *  a(n) is obtained by writing the triangle T(n,k) in row‑major order.
 *  The entry in row n and column k (0‑based) is
 *
 *          T(n,k) = Σ_{j=1..n-k+1}
 *                     ( j /(n+2-j) )
 *                     * C(n+2-j , n-k+1)
 *                     * C(n+2-j , k+1)
 *                     * 2^(j-1)
 *
 *  The code below evaluates a(n) directly from this definition, using
 *  only integer arithmetic and the helper routine binom(n,k) that
 *  computes a binomial coefficient with intermediate cancellation.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Helper: greatest common divisor for seq_t                  */
static seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------ */
/*  Helper: multiplicative binomial coefficient C(n,k) with     */
/*  cancellation to keep intermediate values small.            */
static seq_t binom_n_k(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g = gcd_seq(num, den);
        num /= g; den /= g;

        g = gcd_seq(r, den);
        r /= g;     den /= g;

        r *= num;
        if (den != (seq_t)0) r /= den;
    }
    return r;
}

/* ------------------------------------------------------------ */
/*  Compute a single triangle entry T(r,k).  r and k are 0‑based.*/
static seq_t tri_T(idx_t r, idx_t k)
{
    seq_t total = (seq_t)0;
    idx_t max_j = r - k + 1;

    for (idx_t j = 1; j <= max_j; ++j) {
        /* Basic parameters of this summand */
        idx_t m = r + 2 - j;              /* m = n+2-j  */
        idx_t bk = r - k + 1;             /* choose for first binomial */
        idx_t ck = k + 1;                 /* choose for second binomial */

        /* binomial coefficients */
        seq_t C1 = binom_n_k(m, bk);      /* C(m, n-k+1) */
        seq_t C2 = binom_n_k(m, ck);      /* C(m, k+1)   */

        /* Set up fraction numerator/denominator */
        seq_t num = (seq_t)1;
        seq_t den = (seq_t)m;             /* denominator starts as m = n+2-j */

        /* factor j */
        seq_t jseq = (seq_t)j;
        seq_t g = gcd_seq(jseq, den);
        jseq /= g;  den /= g;
        if (jseq != (seq_t)1) num *= jseq;

        /* factor 2^(j-1) : multiply by 2 only when denominator cannot cancel */
        idx_t exp = j - 1;
        while (exp > 0) {
            if ((den % (seq_t)2) == (seq_t)0) {
                den /= (seq_t)2;
                exp--;
            } else {
                num *= (seq_t)2;
                exp--;
            }
        }

        /* factor C1 */
        g = gcd_seq(C1, den);
        seq_t C1red = C1 / g;
        den /= g;
        if (C1red != (seq_t)1) num *= C1red;

        /* factor C2 */
        g = gcd_seq(C2, den);
        seq_t C2red = C2 / g;
        den /= g;
        if (C2red != (seq_t)1) num *= C2red;

        /* any remaining denominator must divide the numerator */
        if (den != (seq_t)1) num /= den;

        total += num;
    }
    return total;
}

/* ------------------------------------------------------------ */
/*  Public function required by the test harness               */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2   */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;

    /* Column k within that row                      */
    idx_t start_of_row = r * (r + 1) / 2;
    idx_t k = n - start_of_row;

    return tri_T(r, k);
}
