/*
 * A075664  Sum of next n cubes.
 * https://oeis.org/A075664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^7 + 4n^5 + 3n^3)/8
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 760,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative polynomial form: a(n) = n^3 * (n^2 + 1) * (n^2 + 3) / 8 */
seq_t a(idx_t n) {
    /* Convert index to seq_t for all arithmetic */
    seq_t nn   = (seq_t)n;          /* n               */
    seq_t n2   = nn * nn;           /* n^2             */
    seq_t n3   = n2 * nn;           /* n^3             */

    seq_t a_b = n3;                 /* factor a: n^3           */
    seq_t b_b = n2 + (seq_t)1;      /* factor b: n^2 + 1       */
    seq_t c_b = n2 + (seq_t)3;      /* factor c: n^2 + 3       */

    /* Divide the product by 8 (= 2^3) by dividing factors by 2 where possible.
       This keeps intermediate values smaller, especially for 64‑bit builds. */
    for (int i = 0; i < 3; ++i) {
        if ((a_b & (seq_t)1) == 0) {
            a_b >>= 1;
        } else if ((b_b & (seq_t)1) == 0) {
            b_b >>= 1;
        } else { /* c_b must be even in this case */
            c_b >>= 1;
        }
    }

    /* Multiply the remaining factors to obtain the result. */
    seq_t result = a_b;
    result *= b_b;
    result *= c_b;
    return result;
}
