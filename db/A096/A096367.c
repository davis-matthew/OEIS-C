/*
 * A096367  Number of winning paths of length n+1 across an n X n Hex board.
 * https://oeis.org/A096367
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-2)*(n+1)*2^(n-3) - 2^(n-1) + 2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the closed‑form expression for OEIS A096367 */
/* (works for all n >= 3; behavior for smaller n is undefined) */

static inline seq_t pow2(idx_t exp) {
    /* Compute 2^exp as seq_t.
       For seq_t that has a limited width, this may overflow for large exp.
       The caller should only use this for n where the final result
       fits in seq_t. */
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i) {
        result <<= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n < 3) {
        /* Offset starts at 3; for n < 3 the result is undefined */
        return 0;
    }

    /* Cast n to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* Compute 2^(n-3) and 2^(n-1).  Since 2^(n-1) = 2^(n-3) * 4,
       we can reuse the first result. */
    seq_t pow1 = pow2(n - 3);          /* 2^(n-3) */
    seq_t pow2 = pow1 << 2;            /* 2^(n-1) = 4 * 2^(n-3) */

    /* (n-2)*(n+1) as seq_t */
    seq_t term = (sn - 2) * (sn + 1);

    /* Multiply by 2^(n-3) */
    term = term * pow1;

    /* Subtract 2^(n-1) and add 2 */
    term = term - pow2 + 2;

    return term;
}
