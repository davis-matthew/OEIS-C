/*
 * A126815  Ramanujan numbers (A000594) read mod 32.
 * https://oeis.org/A126815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 * sigma_5(n) (mod 32)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset 1 – value for n=0 is irrelevant */

    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    seq_t sigma = sigma_k(n, 5);     /* sigma_5(n) */
    sigma &= (seq_t)31;              /* reduce modulo 32 (1 << 5) */

    /* compute n^3 mod 32 efficiently */
    seq_t n_sq = (n_seq * n_seq) & (seq_t)31;
    seq_t n_cu = (n_sq * n_seq)  & (seq_t)31;

    /* final result mod 32 */
    return (n_cu * sigma) & (seq_t)31;
}
