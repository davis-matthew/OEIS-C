/*
 * A135079  E.g.f. A(x) = Sum_{n>=0} exp(3^n*x)*x^n/n!.
 * https://oeis.org/A135079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)*3^(k*(n-k))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes A135079
 * a(0) = 1
 * a(n) = Σ_{k=0..n} C(n,k) * 3^{k(n-k)}
 *
 * The algorithm uses the recurrence
 *   f(k+1) = f(k) * (n-k)/(k+1) * 3^{(n-1)-2k}
 * where f(k) = C(n,k) * 3^{k(n-k)}.
 * Only the first ⌊n/2⌋ terms are generated; the other terms are
 * obtained by symmetry.  The powers 3^e for 0 ≤ e ≤ n-1 are pre‑computed
 * once per call.
 */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;

    /* pre‑compute powers of three up to exponent n-1 */
    idx_t max_exp = n - 1;
    seq_t *pow3 = (seq_t*)malloc((max_exp + 1) * sizeof(seq_t));
    if (!pow3) return (seq_t)0;          /* out of memory → return 0 */

    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= max_exp; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    seq_t term = (seq_t)1;               /* f(0) */
    seq_t total = (seq_t)0;

    idx_t m = n / 2;                     /* floor(n/2) */

    for (idx_t k = 0; k <= m; ++k) {
        /* add contribution of f(k) */
        if (k * 2 == n)          /* mirrors itself (n even, k = n/2) */
            total += term;
        else                     /* k ≠ n-k, double count */
            total += term + term;

        /* prepare next term if we haven't reached the last */
        if (k == m) break;
        /* term = term * (n-k) / (k+1) * 3^{(n-1)-2k} */
        term = term * (seq_t)(n - k);            /* multiply by (n-k) */
        term = term / (k + 1);                  /* divide by (k+1)  */
        idx_t diff = n - 1 - 2 * k;             /* exponent for 3 */
        term = term * pow3[diff];               /* multiply by 3^diff   */
    }

    free(pow3);
    return total;
}
