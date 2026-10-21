/*
 * A092106  Fractal mountains in base 3.
 * https://oeis.org/A092106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^n Σ_{k=0}^{⌊log_3 i⌋} (-1)^k * ((i / 3^k) % 3)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A092106 (Fractal mountains in base 3). */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;          /* sums with + and - signs */
    seq_t power = 1;                  /* 3^k */
    idx_t  k = 0;
    seq_t n_plus_one = (seq_t)n + 1;  /* number of i values (0..n inclusive) */

    /* iterate over digit positions k while 3^k <= n */
    while (power <= (seq_t)n)
    {
        seq_t period = power * 3;      /* length of one full cycle for digit k */
        seq_t full   = n_plus_one / period;
        seq_t rem    = n_plus_one % period;

        /* sum over complete periods */
        seq_t sum_full = full * period;

        /* sum over the remaining part */
        seq_t k_blocks = rem / power; /* number of full blocks of size 'power' */
        seq_t leftover = rem % power;
        seq_t sum_rem  = 0;

        if (k_blocks > 0)
            sum_rem = (k_blocks * (k_blocks - 1) / 2) * power;
        sum_rem += leftover * k_blocks;

        seq_t sum_k = sum_full + sum_rem;   /* total contribution of digit k */

        if ((k & 1) == 0)   /* even k -> plus sign */
            pos += sum_k;
        else                /* odd k -> minus sign */
            neg += sum_k;

        /* next digit position */
        power *= 3;
        ++k;
    }

    return pos - neg;   /* final result is always non‑negative */
}
