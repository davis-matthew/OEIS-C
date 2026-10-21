/*
 * A217285  Irregular triangle read by rows: T(n,k) is the number of labeled
 *          relations on n nodes with exactly k edges; n>=0, 0<=k<=n^2.
 * https://oeis.org/A217285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 40, u32 n <= 75, u64 n <= 234,
 *            u128 n <= 565, big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"

/* 
 * Function: a
 * ----------------------------
 *   Computes the n‑th entry of OEIS sequence A217285.
 *
 *   The triangle T(r,k) = binomial(r^2,k) is read by rows (r = 0,1,2,…,
 *   k = 0…r^2).  This function finds the row 'r' that contains the
 *   requested index and the offset 'k', then returns binomial(r^2,k).
 *
 *   The binomial coefficient is computed by prime factorisation so the
 *   routine works for any seq_t type (fixed‑width or arbitrary
 *   precision).  No floating point or external libraries are used.
 *
 *   Parameters:
 *      n : the 0‑based index in the flattened sequence
 *
 *   Returns:
 *      seq_t value of a(n)
 */
static seq_t binom_seq(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K > N - K) K = N - K;
    if (K == 0) return 1;
    if (K == 1) return (seq_t)N;

    size_t prime_count;
    uint64_t *primes;
    /* all needed primes are ≤ N (≤ about 1024 for the required range) */
    build_primes_upto(N, &primes, &prime_count);

    seq_t result = 1;
    for (size_t i = 0; i < prime_count; ++i)
    {
        idx_t p = primes[i];

        /* exponent of p in N! */
        idx_t expN = 0, div = p;
        while (div <= N) { expN += N / div; div *= p; }

        /* exponent in K! */
        idx_t expK = 0, divK = p;
        while (divK <= K) { expK += K / divK; divK *= p; }

        /* exponent in (N-K)! */
        idx_t expNK = 0, divNK = p;
        while (divNK <= N - K) { expNK += (N - K) / divNK; divNK *= p; }

        idx_t e = expN - expK - expNK;
        if (e > 0)
        {
            seq_t powv = seq_pow((seq_t)p, e);
            result = seq_mul(result, powv);
        }
    }

    free(primes);
    return result;
}

seq_t a(idx_t n)
{
    idx_t cum = 0;
    idx_t p   = 0;
    /* locate the row containing index n */
    for (;; ++p)
    {
        idx_t row_len = p * p + 1;   /* number of entries in row p */
        if (cum + row_len > n) break;
        cum += row_len;
    }

    idx_t k = n - cum;                 /* offset within that row */
    return binom_seq(p * p, k);
}
