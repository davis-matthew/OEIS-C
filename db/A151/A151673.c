/*
 * A151673  G.f.: Product_{k>=0} (1 + 4*x^(4^k)).
 * https://oeis.org/A151673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥0} (1 + 4*x^{4^k})
 * Range:     u8 n <= 84, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(n) = 4^{∑_{k} d_k(n)} if every base‑4 digit d_k(n) is 0 or 1, */
/* otherwise a(n) = 0. */

seq_t a(idx_t n) {
    idx_t t = n;           /* working copy of the index */
    seq_t value = 1;       /* start with coefficient 1 */

    while (t > 0) {
        idx_t digit = t % 4;     /* get lowest base‑4 digit */
        if (digit > 1)           /* if digit is 2 or 3 → coefficient 0 */
            return 0;
        if (digit == 1)          /* digit 1 contributes a factor of 4 */
            value *= (seq_t)4;
        t /= 4;                  /* shift to next digit */
    }
    return value;
}
