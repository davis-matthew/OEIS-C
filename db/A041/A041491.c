/*
 * A041491  Denominators of continued fraction convergents to sqrt(262).
 * https://oeis.org/A041491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to sqrt(262).
   Recurrent definition: k_-2 = 1, k_-1 = 0,
   k_n   = a_n * k_{n-1} + k_{n-2},
   where a_0 = 16 and for n≥1 a_n follows the period
   5,2,1,2,1,10,16,10,1,2,1,2,5,32  (length 14). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    static const idx_t period[] = {
        5, 2, 1, 2, 1, 10, 16, 10, 1, 2, 1, 2, 5, 32
    };
    const idx_t L = 14;

    seq_t k_prev2 = (seq_t)1;   /* k_{-2} */
    seq_t k_prev1 = (seq_t)0;   /* k_{-1} */
    seq_t k_curr;
    idx_t i;

    for (i = 0; i <= n; ++i) {
        seq_t a_i;
        if (i == 0)
            a_i = (seq_t)16;
        else
            a_i = (seq_t)period[(i - 1) % L];

        k_curr = a_i * k_prev1 + k_prev2;
        k_prev2 = k_prev1;
        k_prev1 = k_curr;
    }

    return k_curr;
}
