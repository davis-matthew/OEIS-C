/*
 * A207167  Number of n X 6 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (19/4)*n^4 + (95/2)*n^3 - (57/4)*n^2 - 19*n (n>=1)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 170, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207167: Number of n × 6 0..1 arrays avoiding specified patterns */

/* Helper to compute the binomial coefficient C(m,4) = m*(m-1)*(m-2)*(m-3)/24.
   The quotient is always an integer for m ≥ 4. */
static inline seq_t binom4(idx_t m)
{
    if (m < 4) return 0;
    seq_t M = (seq_t)m;
    return (M * (M - 1) * (M - 2) * (M - 3)) / 24;
}

seq_t a(idx_t n)
{
    /* Compute the four binomial terms required by the closed‑form expression. */
    const seq_t c1 = 19   * binom4(n + 3);  /* 19 * C(n+3,4)          */
    const seq_t c2 = 266  * binom4(n + 2);  /* 19*14 * C(n+2,4)       */
    const seq_t c3 = 133  * binom4(n + 1);  /* 19*7  * C(n+1,4)       */
    const seq_t c4 = 38   * binom4(n);      /* 19*2  * C(n,4)         */

    /* Sum the positive contributions and subtract the negative ones. */
    const seq_t pos = c1 + c2;
    const seq_t neg = c3 + c4;
    return pos - neg;
}
