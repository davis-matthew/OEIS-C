/*
 * A042192  Numerators of continued fraction convergents to sqrt(621).
 * https://oeis.org/A042192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A042192: Numerators of continued fraction convergents to sqrt(621).
 *
 * Let a_n denote the n‑th numerator (a(0)=24).  The partial quotients of
 * sqrt(621) are
 *   a0 = 24,  a1 = 1,  a2 = 11,  a3 = 2,  a4 = 11,  a5 = 1,  a6 = 48
 *   and thereafter the pattern [1,11,2,11,1,48] repeats.
 *
 * Let p_{-2}=0, p_{-1}=1.  Then for n ≥ 0
 *   p_n = a_n * p_{n-1} + p_{n-2}   (n = 0 uses a0 = 24)
 * and a(n) = p_n.
 *
 * This implementation computes a(n) on demand by iterating the recurrence.
 */

seq_t a(idx_t n) {
    if (n == 0) return 24;          /* a(0) = 24 */

    /* Periodic part of the continued fraction after the integer part. */
    static const seq_t period[6] = {1, 11, 2, 11, 1, 48};

    /* Initial two terms for the recurrence. */
    seq_t p_minus2 = 0;   /* p_{-2} */
    seq_t p_minus1 = 1;   /* p_{-1} */

    /* Compute the first convergent (n = 0). */
    seq_t a0 = 24;
    seq_t p0 = a0 * p_minus1 + p_minus2;   /* 24*1 + 0 = 24 */
    p_minus2 = p_minus1;                   /* p_{-1} <- 1 */
    p_minus1 = p0;                         /* p_{0}  <- 24 */

    /* Iterate for n = 1 .. n. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t a_k = period[(unsigned)((k - 1) % 6)];
        seq_t pk = a_k * p_minus1 + p_minus2;
        p_minus2 = p_minus1;
        p_minus1 = pk;
    }

    return p_minus1; /* p_n */
}
