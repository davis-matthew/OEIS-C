/*
 * A236851  Remultiply n first "upward", from GF(2)[X] to N, and then
 *          remultiply that result back "downward", from N to GF(2)[X]: a(n) =
 *          A234741(A234742(n)).
 * https://oeis.org/A236851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A234741(A234742(n))
 * Range:     u8 n <= 64, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t, clmul, remultiply_downward
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"
#include "../utils/clmul.h"
#include "../utils/remultiply_downward.h"

/*----- Division in GF(2)[X], returns quotient via pointer ----------------*/
static inline bool poly_divides(seq_t p, seq_t d, seq_t *quot)
{
    if (d == 0) return false;
    seq_t rem = p;
    *quot = 0;
    unsigned bd = bitlen_seq_t(d);
    while (rem && bitlen_seq_t(rem) >= bd) {
        unsigned shift = bitlen_seq_t(rem) - bd;
        rem ^= d << shift;
        *quot |= ((seq_t)1 << shift);
    }
    return rem == 0;
}

/*----- Recursive factorisation of a polynomial over GF(2) ------------
   Returns the ordinary product of the integer representations of
   irreducible factors. -------------------------------------------------*/
static seq_t factor_product(seq_t p)
{
    if (p <= 1) return p;   /* 0 or 1 are treated as themselves */

    unsigned deg_p = bitlen_seq_t(p) - 1;
    for (unsigned d_deg = 1; d_deg <= deg_p / 2; ++d_deg) {
        seq_t base = ((seq_t)1 << d_deg);          /* monic part */
        seq_t limit = ((seq_t)1 << d_deg) - 1;     /* lower bits */
        for (seq_t mask = 0; mask <= limit; ++mask) {
            seq_t d = base | mask;                 /* candidate divisor */
            seq_t q;
            if (poly_divides(p, d, &q)) {
                seq_t left  = factor_product(d);
                seq_t right = factor_product(q);
                return left * right;
            }
        }
    }
    /* irreducible polynomial */
    return p;
}

/*----- Upward remultiplication: GF(2)[X] → N ---------------------*/
static seq_t remultiply_upward(seq_t n)
{
    if (n == 0) return 0;
    return factor_product(n);
}

/*----- Entry point ----------------------------------------------------*/
seq_t a(idx_t n)
{
    seq_t up = remultiply_upward((seq_t)n);
    return remultiply_downward(up);
}
