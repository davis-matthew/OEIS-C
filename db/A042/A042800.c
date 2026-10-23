/*
 * A042800  Numerators of continued fraction convergents to sqrt(931).
 * https://oeis.org/A042800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 20, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042800: Numerators of continued‑fraction convergents to sqrt(931).
   Recurrence: a(0)=30, a(1)=31, a(n)=k(n)*a(n-1)+a(n-2)   (n≥2),
   where k(0)=30 and for n≥1: k(n)=coeffs[(n-1)%22+1] with
   coeffs[0] = 30, coeffs[1..22] = {1,1,19,1,5,6,1,1,1,1,2,1,1,1,1,6,5,1,19,1,1,60}. */

static const int coeffs[23] = {
    30, /* k(0) */
    1, 1, 19, 1, 5, 6, 1, 1, 1, 1, 2, 1, 1, 1, 1, 6, 5, 1, 19, 1, 1, 60
};

static inline seq_t kth_coeff(idx_t n) {
    if (n == 0ULL)
        return (seq_t)30;
    /* (n-1)%22 gives 0..21, then +1 gives 1..22 */
    return (seq_t)coeffs[((n - 1ULL) % 22ULL) + 1];
}

seq_t a(idx_t n) {
    if (n == 0ULL)
        return (seq_t)30;
    if (n == 1ULL)
        return (seq_t)31;

    seq_t p_minus2 = (seq_t)0; /* p_{-2} */
    seq_t p_minus1 = (seq_t)1; /* p_{-1} */
    seq_t p = (seq_t)0;        /* current numerator */

    for (idx_t i = 0ULL; i <= n; ++i) {
        seq_t coeff = kth_coeff(i);
        p = coeff * p_minus1 + p_minus2;
        /* shift for next step */
        p_minus2 = p_minus1;
        p_minus1 = p;
    }

    return p; /* p holds a(n) after the loop */
}
