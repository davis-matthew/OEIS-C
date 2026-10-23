/*
 * A042657  Denominators of continued fraction convergents to sqrt(858).
 * https://oeis.org/A042657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(858).
   Recurrence:
     k_{-2} = 1, k_{-1} = 0
     k_n   = a_n * k_{n-1} + k_{n-2}  (n ≥ 0)
   where a_0 = 29 and for n ≥ 1: a_n follows the periodic pattern
   [3, 2, 3, 58] (period length 4).
   a(n) = k_n. */

seq_t a(idx_t n)
{
    /* periodic continued‑fraction terms (starting with a_1). */
    static const int period_len     = 4;
    static const int period_terms[4] = {3, 2, 3, 58};

    /* recurrence state: k_{-2} and k_{-1}. */
    seq_t k_prev2 = 1;   /* k_{-2} */
    seq_t k_prev1 = 0;   /* k_{-1} */
    seq_t k = 0;         /* current value (k_n) */

    for (idx_t i = 0; i <= n; ++i) {
        int a_term;
        if (i == 0) {
            a_term = 29;                /* a_0 */
        } else {
            a_term = period_terms[(i - 1) % period_len];
        }

        k = (seq_t)a_term * k_prev1 + k_prev2;
        k_prev2 = k_prev1;
        k_prev1 = k;
    }

    return k_prev1;
}
