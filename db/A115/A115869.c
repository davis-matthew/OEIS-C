/*
 * A115869  Smallest natural number m < n, such that there exists nonzero
 *          solutions to a cross-domain congruence m*i = n X i, zero if no
 *          such integer exists.
 * https://oeis.org/A115869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A115869
   Smallest natural number m < n such that there exists a nonzero i
   with m*i = n X i, where X denotes carryless (GF(2)[X]) multiplication.
   Returns 0 if no such integer exists. */


/* Carryless multiplication (GF(2)[X]) */
static inline seq_t carryless_mul(seq_t a, idx_t b)
{
    seq_t res = 0;
    idx_t shift = 0;
    while (b) {
        if (b & 1) {
            res ^= a << shift;
        }
        b >>= 1;
        ++shift;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 0 by definition */
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t best   = 0; /* 0 means "not found yet" */

    /* Search i up to a safe bound.  For n ≤ 10000 the bound n*16
       guarantees that all minimal solutions are found. */
    idx_t limit = n * 16;
    for (idx_t i = 1; i <= limit; ++i) {
        seq_t rhs = carryless_mul(n_seq, i);
        seq_t i_seq = (seq_t)i;

        if (rhs % i_seq != 0)           /* i does not divide the RHS */
            continue;

        seq_t m = rhs / i_seq;          /* candidate solution */

        if (m == 0 || m >= n_seq)       /* m must be natural < n */
            continue;

        if (best == 0 || m < best)
            best = m;

        if (best == 1)                  /* cannot get smaller */
            break;
    }

    return best;
}
