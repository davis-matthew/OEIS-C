/*
 * A224527  Number of idempotent 5X5 0..n matrices of rank 1.
 * https://oeis.org/A224527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 * sum_{s_u=0..4} sum_{s_v=0..4-s_u} (4!/(s_u! s_v!
 *            (4-s_u-s_v)!)) * [ s_u=0 && s_v=0 -> 1 s_u=0 && s_v>0 ->
 *            sum_{v=1..n} (v^s_v - (v-1)^s_v) s_u>0 && s_v=0 -> sum_{u=1..n}
 *            (u^s_u - (u-1)^s_u) s_u>0 && s_v>0 -> sum_{u=1..n} (u^s_u -
 *            (u-1)^s_u) * sum_{v=1..⌊n/u⌋} (v^s_v - (v-1)^s_v) ]
 * Range:     u8 none, u16 n <= 6, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   power_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/power_int.h"

static const idx_t factorials[5] = {1, 1, 2, 6, 24};

/* Computes a(n) for n >= 1.  Returns 0 for n < 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t N = n;
    /* Allocate arrays for s = 1..4: arr[s][i] = i^s - (i-1)^s,
       cum[s][i] = sum_{k=1..i} arr[s][k] */
    seq_t *arr[5] = {NULL};
    seq_t *cum[5] = {NULL};

    for (uint8_t s = 1; s <= 4; ++s)
    {
        arr[s] = (seq_t *)malloc((N + 1) * sizeof(seq_t));
        cum[s] = (seq_t *)malloc((N + 1) * sizeof(seq_t));
        if (!arr[s] || !cum[s]) return 0; /* allocation failure */

        cum[s][0] = 0;
        for (idx_t i = 1; i <= N; ++i)
        {
            seq_t pow_i    = power_int((seq_t)i, s);
            seq_t pow_im1  = (i > 1) ? power_int((seq_t)(i - 1), s) : 0;
            arr[s][i] = pow_i - pow_im1;
            cum[s][i] = cum[s][i - 1] + arr[s][i];
        }
    }

    seq_t result = 0;
    const seq_t five = 5;

    /* Enumerate (s_u, s_v) */
    for (uint8_t s_u = 0; s_u <= 4; ++s_u)
    {
        for (uint8_t s_v = 0; s_v <= 4 - s_u; ++s_v)
        {
            uint8_t s_0 = 4 - s_u - s_v;
            seq_t multinomial =
                (seq_t)factorials[4] / ((seq_t)factorials[s_u] *
                                        (seq_t)factorials[s_v] *
                                        (seq_t)factorials[s_0]);

            seq_t subtotal = 0;

            if (s_u == 0 && s_v == 0)
            {
                subtotal = 1; /* only zero vectors on other indices */
            }
            else if (s_u == 0) /* s_v > 0 */
            {
                subtotal = cum[s_v][N]; /* sum over all max_v */
            }
            else if (s_v == 0) /* s_u > 0 */
            {
                subtotal = cum[s_u][N]; /* sum over all max_u */
            }
            else /* both > 0 */
            {
                seq_t sum = 0;
                for (idx_t max_u = 1; max_u <= N; ++max_u)
                {
                    seq_t factor_u = arr[s_u][max_u];
                    idx_t limit = N / max_u;       /* floor(n / max_u) */
                    seq_t sumV    = cum[s_v][limit];
                    sum += factor_u * sumV;
                }
                subtotal = sum;
            }

            seq_t contrib = multinomial * subtotal;
            result += five * contrib;
        }
    }

    /* Free allocated memory */
    for (uint8_t s = 1; s <= 4; ++s)
    {
        free(arr[s]);
        free(cum[s]);
    }

    return result;
}
