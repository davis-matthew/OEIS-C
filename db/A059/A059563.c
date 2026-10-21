/*
 * A059563  Beatty sequence for e + 1/e.
 * https://oeis.org/A059563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * (e + 1/e))
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation approximates e and 1/e by summing their
   Taylor series up to 15 terms, which is accurate enough to
   guarantee the correct floor for all n up to at least 10⁶
   (and well beyond). The approximation is rational with
   denominator K! = 15! = 1307674368000, so all operations
   are done using integer arithmetic only. */

seq_t a(idx_t n)
{
    /* Number of terms in the Taylor series */
    const int K = 15;
    /* factorials up to K! */
    seq_t fact[K + 1];
    fact[0] = (seq_t)1;
    for (int i = 1; i <= K; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    seq_t factK = fact[K];          /* common denominator */

    /* Sum of 1/k! for k = 0..K   ->  e  */
    seq_t num_e   = (seq_t)0;
    /* Sum of (-1)^k / k! for k=0..K -> 1/e  */
    seq_t num_einv = (seq_t)0;

    for (int k = 0; k <= K; ++k) {
        seq_t term = factK / fact[k];         /* integer division, exact */
        num_e += term;
        if ((k & 1) == 0)
            num_einv += term;
        else
            num_einv -= term;
    }

    /* c = e + 1/e as rational numerator/denominator   */
    seq_t num_c = num_e + num_einv;          /* numerator of c */

    /* Compute floor(n * num_c / factK) */
    seq_t n_seq   = (seq_t)n;
    seq_t prod    = n_seq * num_c;
    seq_t result  = prod / factK;

    return result;
}
