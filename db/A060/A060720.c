/*
 * A060720  Singular n X n matrices over GF(5).
 * https://oeis.org/A060720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n^2) - ∏_{k=0}^{n-1} (5^n - 5^k)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A060720: Singular n×n matrices over GF(5) */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1; values below can be undefined */

    /* 5^n */
    seq_t pow5_n = 1;
    for (idx_t i = 0; i < n; ++i) pow5_n *= 5;

    /* Product ∏_{k=0}^{n-1} (5^n - 5^k) */
    seq_t prod = 1;
    seq_t pow5_k = 1;  /* 5^k */
    for (idx_t k = 0; k < n; ++k) {
        seq_t term = pow5_n - pow5_k;
        prod *= term;
        pow5_k *= 5;
    }

    /* 5^(n^2) = (5^n)^n */
    seq_t total = pow_seq_t(pow5_n, n);

    return total - prod;
}
