/*
 * A168847  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=26, a(n)=25*a(n-1) for 2≤n≤19. For n≥20
 *            a(n)=24*(a(n-1)+⋯+a(n-19))–300*a(n-20). This follows from the
 *            rational generating function
 *            (1+2t+⋯+2t^19+t^20)/(1–24t–⋯–24t^19+300t^20)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the first 20 terms. */
    seq_t buf[20];
    buf[0] = (seq_t)1;        /* a(0) */
    buf[1] = (seq_t)26;       /* a(1) */
    for (idx_t i = 2; i <= 19; ++i) {
        buf[i] = buf[i-1] * (seq_t)25;   /* a(i)=25*a(i-1)  */
    }

    if (n < 20)                /* requested term is within the pre‑computed part */
        return buf[n];

    /* Use a circular buffer for the last 20 values. */
    idx_t pos = 0;            /* buf[pos] holds a(current‑20) during the loop */
    seq_t result = 0;         /* will hold the value a(n) after the loop */

    for (idx_t k = 20; k <= n; ++k) {
        /* Sum the 19 most recent values: a(k-1) … a(k-19). */
        seq_t sum = 0;
        for (idx_t j = 1; j < 20; ++j)
            sum += buf[(pos + j) % 20];

        result = 24 * sum - 300 * buf[pos];   /* recurrence */

        buf[pos] = result;                    /* overwrite the oldest value */
        pos = (pos + 1) % 20;                 /* advance to the new oldest */
    }

    return result;
}
