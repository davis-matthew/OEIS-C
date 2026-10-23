/*
 * A184281  Number of distinct n X 4 toroidal 0..2 arrays.
 * https://oeis.org/A184281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(4n)) * Σ_{i=0}^{n-1} Σ_{j=0}^{3} 3^( (4n) /
 *            lcm(n/gcd(n,i), 4/gcd(4,j)) )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

/* GCD for unsigned 64‑bit values */
static inline idx_t _gcd_u64(idx_t a, idx_t b)
{
    while (b) { idx_t t = b; b = a % b; a = t; }
    return a;
}

/* LCM for unsigned 64‑bit values */
static inline idx_t _lcm_u64(idx_t a, idx_t b)
{
    return a / _gcd_u64(a, b) * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    const seq_t three = 3;
    const idx_t total_cells = 4 * n;
    seq_t sum = 0;

    /* Pre‑computed 4/gcd(4,j) for j = 0..3 */
    static const idx_t divb[4] = {1, 4, 2, 4};

    for (idx_t i = 0; i < n; ++i)
    {
        idx_t g_n = _gcd_u64(n, i);
        idx_t rows = n / g_n;            /* n / gcd(n,i) */

        for (idx_t j = 0; j < 4; ++j)
        {
            idx_t L = _lcm_u64(rows, divb[j]);   /*  lcm( rows , 4/gcd(4,j) ) */
            idx_t cycles = total_cells / L;      /* number of orbits of this shift */
            seq_t val = pow_seq(three, (int)cycles);
            sum += val;
        }
    }

    seq_t divisor = total_cells;
    return sum / divisor;
}
