/*
 * A162912  Denominators of drib tree fractions, where drib is the
 *          bit-reversal permutation tree of the Bird tree.
 * https://oeis.org/A162912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(1) = 1; b(1) = 1; For n > 1 let k = ⌊n/2⌋. If n is even: a(n)
 *            = a(k) + b(k); b(n) = a(k); If n is odd: a(n) = b(k); b(n) =
 *            a(k) + b(k); The returned value a(n) is the denominator of the
 *            n‑th fraction in the drib tree
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 92 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void compute_ab(idx_t n, seq_t *ap, seq_t *bp)
{
    if (n == 1) {
        *ap = (seq_t)1;
        *bp = (seq_t)1;
        return;
    }

    idx_t k = n >> 1;          /* floor(n/2) */
    seq_t a_k, b_k;
    compute_ab(k, &a_k, &b_k);

    if ((n & 1) == 0) {        /* even n */
        *ap = a_k + b_k;
        *bp = a_k;
    } else {                   /* odd n */
        *ap = b_k;
        *bp = a_k + b_k;
    }
}
#define MULTIPLY_BY_2(v)(v<<1)

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* below offset, value irrelevant */
    seq_t a_val, b_val;
    compute_ab(n, &a_val, &b_val);
    return a_val;
}
