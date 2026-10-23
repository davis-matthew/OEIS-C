/*
 * A034742  Dirichlet convolution of Moebius function mu(n) (A008683) with
 *          Catalan numbers (A000108).
 * https://oeis.org/A034742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * Catalan(d-1) (Catalan(k)=C_k)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   catalan, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"   /* seq_t mobius(idx_t m);  returns -1,0,1 */
#include "../utils/catalan.h"  /* seq_t catalan(idx_t k);  returns C_k */

seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = 0, etc.  Defined only for n>=1 */
    if (n < 1) return 0;

    seq_t add_sum = 0;   /* sum of terms with mu = +1  */
    seq_t sub_sum = 0;   /* sum of terms with mu = -1  */

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        idx_t e = n / d;                      /* the paired divisor */

        /* process divisor d  -> mu(n/d) = mu(e) */
        int mu_d = (int)mobius(e);
        seq_t cat_d = catalan(d - 1);          /* C_{d-1} */
        if (mu_d ==  1) add_sum += cat_d;
        else if (mu_d == -1) sub_sum += cat_d;

        if (e != d) {                         /* process divisor e  -> mu(n/e)=mu(d) */
            int mu_e = (int)mobius(d);
            seq_t cat_e = catalan(e - 1);      /* C_{e-1} */
            if (mu_e ==  1) add_sum += cat_e;
            else if (mu_e == -1) sub_sum += cat_e;
        }
    }

    return add_sum - sub_sum;                 /* result is non‑negative */
}
