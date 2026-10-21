/*
 * A237009  Number of (n+1)X(2+1) 0..3 arrays with the sum of all four
 *          elements of every 2X2 subblock equal.
 * https://oeis.org/A237009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Define for each n ≥ 1 the number a(n) of (n+1)×3 arrays with
 *            entries 0..3 such that every 2×2 sub‑block has the same sum. A
 *            combinatorial analysis gives a(n) = Σ_{a=0..6} Σ_{b=0..6}
 *            Σ_{p=-3..3} E(a,b,p)^{⌈(n+1)/2⌉} · O(a,b,p)^{⌊(n+1)/2⌋} where
 *            E(a,b,p) = max(0, min(3, a, 3−p) − max(0, a−3, −p) +...
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   min_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/min_int.h"

static inline int max_int(int a, int b, int c)
{
    int m = a > b ? a : b;
    return m > c ? m : c;
}

/* Fast binary exponentiation for seq_t types.  The base is a seq_t,
   the exponent a uint64_t index. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1ULL)
            result = result * base;
        base = base * base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* number of rows */
    idx_t rows = n + 1ULL;
    /* counts of even / odd‑indexed rows (0‑based) */
    idx_t evRows = (rows + 1ULL) / 2ULL;   /* ceil(rows/2) */
    idx_t odRows = rows / 2ULL;            /* floor(rows/2) */

    seq_t ans = 0;

    for (int a = 0; a <= 6; ++a)
    {
        for (int b = 0; b <= 6; ++b)
        {
            for (int p = -3; p <= 3; ++p)
            {
                /* Even rows constraints */
                int low_e = max_int(0, a - 3, -p);
                int high_e = min_int(3, a, 3 - p);
                int E = (low_e <= high_e) ? (high_e - low_e + 1) : 0;

                if (E == 0)
                    continue;   /* no sequence for this parity */

                /* Odd rows constraints */
                int low_o = max_int(0, b - 3, p);
                int high_o = min_int(3, b, 3 + p);
                int O = (low_o <= high_o) ? (high_o - low_o + 1) : 0;

                if (O == 0)
                    continue;   /* no sequence for this parity */

                /* Contribution of this triple (a,b,p) */
                seq_t term_even = pow_seq((seq_t)E, evRows);
                seq_t term_odd  = pow_seq((seq_t)O, odRows);
                ans = ans + term_even * term_odd;
            }
        }
    }

    return ans;
}
