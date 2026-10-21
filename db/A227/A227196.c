/*
 * A227196  a(n) = first i >= 1 for which the Kronecker symbol K(i,n) is not
 *          +1 (i.e., is either 0 or -1), 0 if no such i exists.
 * https://oeis.org/A227196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first i ≥ 1 with Kronecker symbol K(i,n) ≠ +1; computed
 *            by a search
 * Range:     u8 n <= 9802, u16 n <= 9802, u32 n <= 9802, u64 n <= 9802,
 *            u128 n <= 9802, big any n
 * Helpers:   gcd64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/gcd64.h"

/* Euclidean gcd for uint64_t */

seq_t a(idx_t n)
{
    /* Handle the unique case n = 1 : a(1) = 0 */
    if (n == 1)
        return 0;

    uint64_t nn = (uint64_t)n;

    /* Even n → the first non‑+1 value is always i = 2 */
    if ((nn & 1) == 0)
        return 2;

    /* Factorise the odd integer nn.  Distinct primes are stored in primes[];
       exponents are stored in exps[].  The number of distinct factors is cntPr. */
    uint64_t tmp = nn;
    uint64_t primes[64];
    int exps[64];
    int cntPr = 0;

    for (uint64_t d = 3; d <= tmp / d; d += 2) {
        if (tmp % d == 0) {
            int e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++e;
            }
            primes[cntPr] = d;
            exps[cntPr] = e;
            ++cntPr;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[cntPr] = tmp;
        exps[cntPr]   = 1;
        ++cntPr;
    }

    /* For the Kronecker/Jacobi symbol only primes with odd exponent matter. */
    uint64_t oddPr[64];
    int oddCnt = 0;
    for (int i = 0; i < cntPr; ++i) {
        if (exps[i] & 1) {
            oddPr[oddCnt++] = primes[i];
        }
    }

    /* Scan i = 2,3,4,...  until the symbol is not +1. */
    for (uint64_t i = 2; ; ++i) {
        /* If i shares a factor with nn the symbol becomes 0. */
        if (gcd64(i, nn) != 1)
            return (seq_t)i;

        /* Compute the product of Legendre symbols over odd‑exponent primes. */
        int prod = 1;
        for (int j = 0; j < oddCnt; ++j) {
            int s = jacobi_symbol(i, oddPr[j]);   /* -1, 0, or 1 */
            prod *= s;
            if (prod == 0)          /* already not +1 */
                break;
        }

        if (prod != 1)              /* product is 0 or -1 */
            return (seq_t)i;
    }
}
