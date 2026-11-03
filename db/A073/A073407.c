/*
 * A073407  Let phi_m(x) denote the Euler totient function applied m times to
 *          x. Sequence gives the minimum number of iterations m such that
 *          phi_m(n) divides n.
 * https://oeis.org/A073407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min{m≥1 : φ^m(n) divides n} (iterated Euler totient)
 * Range:     u8 n <= 823, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9875 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* Start with φ^1(n) and look for the first iterated totient
       that divides the original n.  φ^0(n)=n trivially divides n,
       but the definition asks for at least one iteration. */
    idx_t cur = n;
    for (int m = 1; ; ++m)
    {
        /* φ iterated m‑times is stored in cur.  phi() returns a seq_t,
           but cur is an idx_t and φ(cur) ≤ cur, so the conversion is safe. */
        cur = (idx_t)totient(cur);

        /* Check divisibility using the original n (idx_t) and the
           iterated totient (also an idx_t).  Return the minimal m. */
        if (n % cur == 0)
            return (seq_t)m;
    }
}
