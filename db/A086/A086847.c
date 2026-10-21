/*
 * A086847  a(n) = gcd(A001608(n), n), where A001608 = Perrin sequence.
 * https://oeis.org/A086847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(Perrin(n), n) Perrin(0)=3, Perrin(1)=0, Perrin(2)=2,
 *            Perrin(n)=Perrin(n-2)+Perrin(n-3)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for the problem, safe fallback */
    idx_t a0 = 3 % n;                    /* Perrin(n-3) */
    idx_t a1 = 0 % n;                    /* Perrin(n-2) */
    idx_t a2 = 2 % n;                    /* Perrin(n-1) */

    if (n == 1)                          /* Perrin(1) = 0 */
        return gcd((seq_t)a1, (seq_t)n);

    if (n == 2)                          /* Perrin(2) = 2 */
        return gcd((seq_t)a2, (seq_t)n);

    /* Compute Perrin(n) modulo n efficiently */
    idx_t pk;
    for (idx_t i = 3; i <= n; ++i) {
        pk = (a0 + a1) % n;              /* Perrin(i) = Perrin(i-2)+Perrin(i-3) */
        a0 = a1;                         /* shift the window */
        a1 = a2;
        a2 = pk;
    }
    /* gcd of Perrin(n) and n (n divides into its own gcd value) */
    return gcd((seq_t)a2, (seq_t)n);
}
