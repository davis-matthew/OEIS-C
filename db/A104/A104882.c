/*
 * A104882  Diagonal sums of number triangle A104881.
 * https://oeis.org/A104882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} Σ_{j=0..k} (n-2k)^(k-j)
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 28, u64 n <= 46, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    idx_t maxk = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t t_idx = n - 2 * k;          /* n-2k, non–negative */
        seq_t base = (seq_t)t_idx;        /* convert to seq_t */

        seq_t inner = 1;      /* sum of powers, start with r = 0 term = 1 */
        seq_t term  = 1;      /* current power base^r */

        /* add remaining powers base^1 ... base^k */
        for (idx_t r = 1; r <= k; ++r)
        {
            term *= base;     /* term = base^r */
            inner += term;    /* add to the inner sum */
        }

        total += inner;       /* add to the overall sum */
    }

    return total;
}
