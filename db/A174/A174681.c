/*
 * A174681  Half-sums (averages) of two primes with prime subscripts.
 * https://oeis.org/A174681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sequence of distinct values (q_i + q_j)/2 where q_k =
 *            p_{p_k} (k‑th prime taken at a prime index)
 * Range:     u8 none, u16 n <= 7, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   compare_seq_t, is_prime, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function precomputes the first 20 000 terms once, which is
   sufficient for all n up to about 10 000. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/sieve_primes.h"
#include "../utils/compare_seq_t.h"

static seq_t *sequence = NULL;
static size_t seq_len    = 0;
static bool ready        = false;

static void precompute(void)
{
    if (ready) return;

    /* Parameters: number of terms to keep, number of q‑values to generate,
       limit for sieving primes. */
    const size_t PRECOMP  = 20000;
    const size_t Q_TARGET = 2500;
    const idx_t  LIMIT    = 500000;

    /* --- Generate all primes up to LIMIT -------------------------------- */
    idx_t *primes_all = NULL;
    size_t primes_all_cnt = 0;
    sieve_primes(LIMIT, &primes_all, &primes_all_cnt);

    /* --- Build list of primes whose indices are prime (q‑values) -------- */
    seq_t *Q = (seq_t *)malloc(Q_TARGET * sizeof(seq_t));
    size_t Qlen = 0;
    for (idx_t idx = 1; idx <= primes_all_cnt && Qlen < Q_TARGET; ++idx) {
        if (is_prime(idx)) {
            Q[Qlen++] = (seq_t)primes_all[idx - 1];  /* primes_all[0] is p_1 */
        }
    }

    /* --- Compute all possible half‑sums --------------------------------- */
    size_t num_pairs = Qlen * (Qlen + 1) / 2;
    seq_t *sums = (seq_t *)malloc(num_pairs * sizeof(seq_t));
    size_t pos = 0;
    for (size_t i = 0; i < Qlen; ++i) {
        for (size_t j = i; j < Qlen; ++j) {
            sums[pos++] = (Q[i] + Q[j]) / 2;
        }
    }

    /* --- Sort and deduplicate ------------------------------------------- */
    qsort(sums, num_pairs, sizeof(seq_t), compare_seq_t);
    seq_t *unique = (seq_t *)malloc(num_pairs * sizeof(seq_t));
    size_t uniq   = 0;
    for (size_t i = 0; i < num_pairs; ++i) {
        if (i == 0 || compare_seq_t(&sums[i - 1], &sums[i]) != 0) {
            unique[uniq++] = sums[i];
        }
    }

    /* --- Keep first PRECOMP terms --------------------------------------- */
    size_t limit = (uniq > PRECOMP) ? PRECOMP : uniq;
    sequence = (seq_t *)malloc(limit * sizeof(seq_t));
    for (size_t i = 0; i < limit; ++i) {
        sequence[i] = unique[i];
    }
    seq_len = limit;

    /* --- Clean up -------------------------------------------------------- */
    free(sums);
    free(unique);
    free(Q);
    free(primes_all);

    ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset 1: a(1) is first term */
    precompute();
    if (n <= seq_len) return sequence[n - 1];
    /* For safety, if requested n is too large, return last term */
    return sequence[seq_len - 1];
}
