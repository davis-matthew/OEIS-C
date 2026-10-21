/*
 * A211059  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          positive determinant.
 * https://oeis.org/A211059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - Z)/2, where Z = # {(a,b,c,d) | a,b,c,d∈[1..n], a*d
 *            = b*c}. * Z is computed via the identity * Z = Σ_{g=1}^{n}
 *            Σ_{k=1}^{⌊n/g⌋} ⌊n/k⌋ · F(k) * with F(1)=1 and F(k)=2·φ(k) for
 *            k>1. * φ(k) is Euler's totient function. The totients are
 *            pre‑computed by a * simple sieve. Complexity is O(n...
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for non‑positive indices */

    /* allocate array for φ(1…n) – small, <10000 for the test harness */
    size_t sz = (size_t)n + 1;
    uint64_t *phi = (uint64_t *)malloc(sz * sizeof(uint64_t));
    if (!phi) return 0;                   /* allocation failed – unlikely */

    /* initialise φ(i) = i, then apply the standard totient sieve */
    for (size_t i = 0; i < sz; ++i) phi[i] = i;
    for (size_t i = 2; i < sz; ++i) {
        if (phi[i] == i) {                /* i is prime */
            for (size_t j = i; j < sz; j += i)
                phi[j] -= phi[j] / i;
        }
    }

    /* compute Z = Σ_{g=1}^n Σ_{k=1}^{⌊n/g⌋} ⌊n/k⌋ · F(k) */
    seq_t Z = 0;
    for (idx_t g = 1; g <= n; ++g) {
        idx_t m = n / g;                  /* upper bound for k */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t fk = (k == 1) ? 1
                               : (seq_t)2 * (seq_t)phi[(size_t)k];
            Z += (seq_t)(n / k) * fk;
        }
    }

    free(phi);

    /* compute n^4 as seq_t */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;

    /* final formula: a(n) = (n^4 - Z)/2 */
    return (n4 - Z) / 2;
}
