/*
 * A103641  Number of tilings of the Aztec dungeon D_n.
 * https://oeis.org/A103641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+6) = 13^(4n+12) * a(n); Initial values: a(0)=1, a(1)=2,
 *            a(2)=13, a(3)=2197, a(4)=742586, a(5)=815730721
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Fast exponentiation for seq_t.  */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/*  Computes a(n) as described above.  */
seq_t a(idx_t n)
{
    /*  Initial values for residues modulo 6.  */
    static const seq_t init[6] = {
        (seq_t)1,          /* a(0) */
        (seq_t)2,          /* a(1) */
        (seq_t)13,         /* a(2) */
        (seq_t)2197,       /* a(3) */
        (seq_t)742586,     /* a(4) */
        (seq_t)815730721   /* a(5) */
    };

    idx_t k = n / 6;      /* Number of 6‑step jumps. */
    idx_t r = n % 6;      /* Residue modulo 6. */

    if (k == 0) return init[r];   /* Small indices – return the base value. */

    /*  Total exponent of 13: 4*k*r + 12*k^2.  */
    idx_t exp = 4 * k * r + 12 * k * k;

    seq_t pow13 = pow_seq((seq_t)13, exp);
    return pow13 * init[r];
}
