/*
 * A169274  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (t^29 + 2*t^28 + ... + 2*t + 1)/(190*t^29 - 19*t^28 - ...
 *            - 19*t + 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: one may this as    a(n) = 19*∑_{k=1}^{28} a(n-k) – 190*a(n-29)   for n ≥ 29  */
/* with initial terms a(0)=1, a(n)=2+19*∑_{k=0}^{n-1} a(k)  for 1 ≤ n ≤ 28.      */
seq_t a(idx_t n)
{
    seq_t buf[29] = {0};
    seq_t sumAll = 0;   /* sum of all previous a(k)   */
    seq_t sumLast28 = 0;/* sum of the last 28 a(k)     */
    seq_t res = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ai;
        if (i == 0) {                     /* a(0) = 1 */
            ai = 1;
        }
        else if (i <= 28) {                /*  a(i) = 2 + 19*sumAll  */
            ai = 2 + 19*sumAll;
        }
        else {                            /* recurrence for i ≥ 29  */
            seq_t old = buf[i % 29];      /* a(i-29) */
            ai = 19*sumLast28 - 190*old;
        }

        /* store ai in buffer              */
        buf[i % 29] = ai;

        /* update sums                       */
        sumAll = sumAll + ai;
        sumLast28 = sumLast28 + ai;
        if (i >= 28) {
            seq_t toRemove = buf[(i - 28) % 29];
            sumLast28 = sumLast28 - toRemove;
        }

        if (i == n) res = ai;
    }
    return res;
}
