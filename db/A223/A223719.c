/*
 * A223719  Number of n X 2 0..2 arrays with rows, antidiagonals and columns
 *          unimodal.
 * https://oeis.org/A223719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) * - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9),
 *            a(1)=9, * a(2)=81, ..., a(9)=292681
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 33, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: sequence values grow polynomially; recurrence uses only
 * unsigned arithmetic on seq_t. Each n is computed in at most O(n) steps. */
#include <stdint.h>
#include <stdbool.h>

static const seq_t init[10] =
{
    0,              // unused, a(0) not defined
    (seq_t)9,       // a(1)
    (seq_t)81,      // a(2)
    (seq_t)484,     // a(3)
    (seq_t)2116,    // a(4)
    (seq_t)7396,    // a(5)
    (seq_t)21904,   // a(6)
    (seq_t)57121,   // a(7)
    (seq_t)134689,  // a(8)
    (seq_t)292681   // a(9)
};

static const seq_t coeff_abs[9] = {
    (seq_t)9,
    (seq_t)36,
    (seq_t)84,
    (seq_t)126,
    (seq_t)126,
    (seq_t)84,
    (seq_t)36,
    (seq_t)9,
    (seq_t)1
};

static const int16_t coeff_pos[9] = {
    1,  // +9
    0,  // -36
    1,  // +84
    0,  // -126
    1,  // +126
    0,  // -84
    1,  // +36
    0,  // -9
    1   // +1
};

seq_t a(idx_t n)
{
    if (n <= 9)
        return init[n];

    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i)   /* buf[0] = a(1), ..., buf[8] = a(9) */
        buf[i] = init[i + 1];

    int idx = 8;   /* index of the most recent term in buf */

    for (idx_t cur = 10; cur <= n; ++cur)
    {
        seq_t posSum = 0;
        seq_t negSum = 0;

        for (int k = 0; k < 9; ++k)
        {
            /* term at distance k from the current term: a(cur - k - 1) */
            int idx_term = idx - k;
            idx_term %= 9;
            if (idx_term < 0) idx_term += 9;
            seq_t prod = buf[idx_term] * coeff_abs[k];
            if (coeff_pos[k])
                posSum += prod;
            else
                negSum += prod;
        }

        seq_t next = posSum - negSum; /* posSum >= negSum by definition */

        idx = (idx + 1) % 9;
        buf[idx] = next;
    }

    return buf[idx];
}
