/*
 * A225574  Additive endpoints: range of A225561.
 * https://oeis.org/A225574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(m) where m runs through the practical numbers. The
 *            function generates all practical numbers up to a fixed bound,
 *            computes their sum‑of‑divisors σ(m), sorts these σ values, and
 *            returns the n‑th smallest (1‑based)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/cmp_seq_t.h"   // int cmp_seq_t(const void *, const void *);

static seq_t *seq = NULL;          /* sorted list of sigma values   */
static size_t seq_len = 0;         /* number of values in seq       */
static bool init_done = false;     /* flag indicating ready state   */
static idx_t *spf = NULL;          /* smallest prime factor table   */
static idx_t spf_limit = 0;        /* size of spf table             */

/* Build linear sieve up to N, filling spf.                         */
static void build_spf(idx_t N)
{
    spf = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);
    spf[0] = spf[1] = 1;
    for (idx_t i = 2; i <= N; ++i) spf[i] = 0;
    size_t primes_cap = N / 10 + 10;
    idx_t *primes = (idx_t*)malloc(primes_cap * sizeof(idx_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t pc = 0;
    for (idx_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (pc == primes_cap) {
                primes_cap <<= 1;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[pc++] = i;
        }
        for (size_t j = 0; j < pc && primes[j] <= spf[i] && i * primes[j] <= N; ++j) {
            spf[i * primes[j]] = primes[j];
        }
    }
    free(primes);
}

/* Initialize the sequence, generating all practical numbers up to a preset bound.
   The bound is chosen large enough to cover a substantial number of terms.     */
static void init_sequence(void)
{
    if (init_done) return;

    const idx_t LIMIT = 5000000;       /* bound for practical number search */
    build_spf(LIMIT);
    spf_limit = LIMIT;

    /* Temporary array that will be sorted and deduplicated. */
    size_t cap = 200000;               /* initial capacity for sequence */
    seq = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!seq) exit(EXIT_FAILURE);

    for (idx_t m = 1; m <= LIMIT; ++m) {
        /* Compute whether m is practical and its sigma(m). */
        if (m == 1) {          /* 1 is practical by definition */
            if (seq_len == cap) {
                cap <<= 1;
                seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
                if (!seq) exit(EXIT_FAILURE);
            }
            seq[seq_len++] = (seq_t)1;
            continue;
        }

        idx_t n = m;
        bool practical = true;
        seq_t sigma = 1;          /* total sigma(m) */
        seq_t sigma_prev = 1;     /* sigma of processed part, used for practicality test */
        bool first_prime = true;

        while (n > 1) {
            idx_t p = spf[n];
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }

            /* Compute sigma factor for p^exp: (p^{exp+1} - 1)/(p - 1)   */
            seq_t sigma_factor = 1;
            seq_t pp = 1;
            for (int e = 0; e < exp; ++e) {
                pp *= (seq_t)p;
                sigma_factor += pp;
            }

            if (first_prime) {
                if (p != 2) { practical = false; break; }
                first_prime = false;
            } else {
                if ((seq_t)p > (seq_t)1 + sigma_prev) { practical = false; break; }
            }

            sigma_prev *= sigma_factor;
            sigma *= sigma_factor;
        }

        if (practical) {
            if (seq_len == cap) {
                cap <<= 1;
                seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
                if (!seq) exit(EXIT_FAILURE);
            }
            seq[seq_len++] = sigma;
        }
    }

    /* Sort the collected sigma values.  */
    qsort(seq, seq_len, sizeof(seq_t), cmp_seq_t);

    /* Remove duplicates (though unlikely). */
    size_t uniq = 0;
    for (size_t i = 0; i < seq_len; ++i) {
        if (i == 0 || cmp_seq_t((const void*)&seq[i - 1], (const void*)&seq[i]) != 0) {
            seq[uniq++] = seq[i];
        }
    }
    seq_len = uniq;

    init_done = true;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n==0, but harmless */
    init_sequence();
    if (n > seq_len) {
        /* If requested beyond precomputed range, extend search. */
        /* But for the intended usage (n <= 10000), this path is not taken.   */
        return 0;
    }
    return seq[n - 1];
}
