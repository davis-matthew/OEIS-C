/*
 * A188064  Partial sums of wt(n)! where wt(n) is the Hamming weight of n
 *          (A000120).
 * https://oeis.org/A188064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^n wt(k)! where wt(k) is the Hamming weight of k
 * Range:     u8 n <= 30, u16 n <= 253, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the partial sums of factorials of Hamming weights. */
seq_t a(idx_t n)
{
    /* Pre‑compute factorials of small integers (0! … 64!). */
    static seq_t fact[65];
    static int fact_init = 0;
    if (!fact_init) {
        fact[0] = 1;
        for (unsigned w = 1; w < 65; ++w) {
            fact[w] = fact[w - 1] * (seq_t)w;
        }
        fact_init = 1;
    }

    seq_t sum = fact[0];   /* term for k = 0, wt(0) = 0, so 0! = 1. */

    for (idx_t k = 1; k <= n; ++k) {
        unsigned int w = __builtin_popcountll((unsigned long long)k);
        sum += fact[w];
    }

    return sum;
}
