/*
 * A124403  a(n) = -1 + Sum_{i=1..n} Sum_{j=1..n} i^j.
 * https://oeis.org/A124403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - 1 + Sum_{j=2..n} j * (j^n - 1) / (j - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A124403. */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t sum   = (seq_t)0;
    seq_t nseq  = (seq_t)n;

    for (idx_t j = (idx_t)2; j <= n; ++j) {
        seq_t base  = (seq_t)j;
        seq_t pow   = pow_seq(base, n);
        seq_t num   = base * (pow - (seq_t)1);
        seq_t den   = base - (seq_t)1;
        seq_t term  = num / den;          /* exact division */
        sum += term;
    }

    return sum + (nseq - (seq_t)1);
}
