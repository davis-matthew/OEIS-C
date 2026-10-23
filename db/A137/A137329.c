/*
 * A137329  a(n)=-4a(n-4).
 * https://oeis.org/A137329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = init[n mod 4] * (-4)^{floor(n/4)}
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = -4*a(n-4), with a(0)=0, a(1)=2, a(2)=3, a(3)=1 */

static seq_t pow_neg4(idx_t k)
{
    seq_t res   = 1;
    const seq_t base = -4;
    for (idx_t i = 0; i < k; ++i) {
        res *= base;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* initial values for residues 0..3 */
    static const seq_t init[4] = {0, 2, 3, 1};

    seq_t mult = pow_neg4(n >> 2);
    return init[n & 3] * mult;
}
