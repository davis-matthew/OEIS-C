/*
 * A063236  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          85 ).
 * https://oeis.org/A063236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A063236
   Dimension of the space of weight 2n cuspidal newforms for Gamma_0(85)
   Recurrence for n ≥ 6:
     a(1) = 5
     a(2) = 16
     a(3) = 28
     a(4) = 36
     a(5) = 48
     a(n) = a(n-1) + a(n-3) – a(n-4)   (n ≥ 6)
*/
seq_t a(idx_t n) {
    /* Handle indices below the offset – behaviour arbitrary */
    if (n == 0) return (seq_t)0;

    /* Initial values */
    static const seq_t init[5] = {
        (seq_t)5,  /* a(1) */
        (seq_t)16, /* a(2) */
        (seq_t)28, /* a(3) */
        (seq_t)36, /* a(4) */
        (seq_t)48  /* a(5) */
    };

    if (n <= 5) return init[n-1];

    seq_t v0 = init[0]; /* a(n-5) */
    seq_t v1 = init[1]; /* a(n-4) */
    seq_t v2 = init[2]; /* a(n-3) */
    seq_t v3 = init[3]; /* a(n-2) */
    seq_t v4 = init[4]; /* a(n-1) */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-3) - a(i-4) */
        seq_t next = v4 + v2 - v1;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }
    return v4;
}
