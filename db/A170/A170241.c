/*
 * A170241  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 for 1 ≤ i ≤ 39: a(i) = 2 + 38 * ∑_{k=0}^{i-1} a(k)
 *            a(40) = 1 + 38 * ∑_{k=1}^{39} a(k) – 741 * a(0) for i ≥ 41: a(i)
 *            = 38 * ∑_{k=1}^{39} a(i-k) – 741 * a(i-40)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static storage for terms up to 10000 (max requested in the tests) */
    static seq_t seq[10001];
    static idx_t  last    = 0;          /* highest index computed so far   */
    static seq_t  total   = 0;          /* sum a[0..last]                   */
    static seq_t  window  = 0;          /* sum of last 39 terms for i ≥ 41   */
    static bool   ready   = false;      /* one‑time initialisation           */

    if (!ready) {                        /* initialise the first term */
        seq[0] = (seq_t)1;
        total  = seq[0];
        last   = 0;
        window = 0;                      /* will be set once i==41 is reached */
        ready  = true;
    }

    /* compute missing terms up to n (if needed) */
    while (last < n) {
        idx_t  i = last + 1;
        seq_t  val;

        if (i <= 39) {                     /* 1 ≤ i ≤ 39 */
            val = (seq_t)2 + (seq_t)38 * total;
        } else if (i == 40) {              /* i = 40 */
            seq_t sum_1_39 = total - seq[0];
            val = (seq_t)1 + (seq_t)38 * sum_1_39 - (seq_t)741 * seq[0];
            /* initialise window: a[2] + … + a[40]   */
            window = total - seq[0] - seq[1];
        } else {                           /* i ≥ 41 */
            val = (seq_t)38 * window - (seq_t)741 * seq[i - 40];
            window = window - seq[i - 39] + val;   /* slide the window   */
        }

        seq[i]  = val;
        total += val;
        last   = i;
    }

    return seq[n];
}
