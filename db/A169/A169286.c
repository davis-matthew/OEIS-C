/*
 * A169286  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   for 1 ≤ n ≤ 28:  a(n)=31*∑_{k=1}^{n} a(n‑k)+2;
   for n ≥ 29:      a(n)=31*∑_{k=1}^{28} a(n‑k)−496*a(n‑29); */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t ring[29] = {0};
    ring[0] = (seq_t)1;
    seq_t sum28 = ring[0];

    for (idx_t i = 1; i <= n; ++i) {
        seq_t ai;
        if (i <= 28) {
            ai = (seq_t)31 * sum28 + (seq_t)2;
        } else {
            idx_t idx = (i - 29) % 29;
            sum28 -= ring[idx];
            ai = (seq_t)31 * sum28 - (seq_t)496 * ring[idx];
        }
        ring[i % 29] = ai;
        sum28 += ai;
    }

    return ring[n % 29];
}
