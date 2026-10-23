/*
 * A118446  Number of tree-rooted maps of genus 2 with n edges: rooted maps
 *          with a distinguished spanning tree on an orientable surface of
 *          genus 2.
 * https://oeis.org/A118446
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-3)*(n-2)^2*(n-1)*n*(5*n^2+n+6) * binomial(2*n,n)^2 /
 *            (5760*(n+1)*(2*n-3)*(2*n-1)) The computation is performed by
 *            factorising the numerator and denominator and cancelling common
 *            factors with gcd, so that intermediate values never overflow
 *            seq_t
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

static void reduce_mul(seq_t *res, seq_t *den, seq_t term) {
    seq_t g = gcd(term, *den);
    if (g > 1) {
        term /= g;
        *den /= g;
    }
    *res *= term;
}

seq_t a(idx_t n) {
    if (n < 4) return (seq_t)0;   /* values below offset are undefined */

    seq_t res = 1;

    /* Construct the denominator */
    seq_t den = 5760;
    den *= (n + 1);
    den *= (2 * n - 3);
    den *= (2 * n - 1);

    /* Numerator terms */

    /* (n-3) */
    reduce_mul(&res, &den, n - 3);

    /* (n-2)^2  -> multiply by (n-2) twice, cancelling with den each time */
    seq_t t = n - 2;
    reduce_mul(&res, &den, t);
    reduce_mul(&res, &den, t);

    /* (n-1) */
    reduce_mul(&res, &den, n - 1);

    /* n */
    reduce_mul(&res, &den, n);

    /* 5*n^2 + n + 6 */
    seq_t poly = 5 * n * n + n + 6;
    reduce_mul(&res, &den, poly);

    /* binomial(2*n,n)^2 – handle the two factors separately */
    seq_t bin = binomial(2 * n, n);
    reduce_mul(&res, &den, bin);
    reduce_mul(&res, &den, bin);

    /* Any remaining denominator factor must divide the result exactly. */
    if (den > 1) res /= den;

    return res;
}
