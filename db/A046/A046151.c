/*
 * A046151  a(n) = n*phi(n) - 1.
 * https://oeis.org/A046151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*phi(n) - 1
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* safety for possible non‑positive calls */
        return 0;

    seq_t n_seq = (seq_t)n;      /* convert index to seq_t */
    seq_t phi_n = phi(n);        /* use the pre‑supplied totient routine */

    return n_seq * phi_n - 1;    /* exact arithmetic in seq_t */
}
