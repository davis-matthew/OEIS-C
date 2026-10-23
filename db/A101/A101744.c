/*
 * A101744  Triangular numbers which are 10-almost primes.
 * https://oeis.org/A101744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangular numbers that are 10‑almost primes.
   The function searches triangular numbers T(k)=k*(k+1)/2 for those
   whose prime factorisation contains exactly 10 prime factors
   (counted with multiplicity).  It pre‑computes a sieve of smallest
   prime factors up to a fixed bound and keeps a dynamic list of the
   qualifying values, so any n ≥ 1 can be answered in O(1) after
   the first invocation. */

#include <stdint.h>
#include <stdlib.h>

/* Upper bound for k used in the sieve (enough for ~10 000 terms). */
static const idx_t SIEVE_MAX_K = 8000000;

/* Smallest prime factor and multiplicity count arrays. */
static uint32_t *spf   = NULL;
static uint32_t *omega = NULL;
static idx_t spf_limit = 0;

/* Simple linear sieve that fills spf[] and omega[]. */
static void init_sieve(void)
{
    if (spf_limit >= SIEVE_MAX_K) return;

    spf   = (uint32_t *)malloc((SIEVE_MAX_K + 1) * sizeof(uint32_t));
    omega = (uint32_t *)malloc((SIEVE_MAX_K + 1) * sizeof(uint32_t));
    if (!spf || !omega) exit(1);

    spf[0]   = 0;
    spf[1]   = 1;
    omega[0] = 0;
    omega[1] = 0;

    uint32_t *primes = (uint32_t *)malloc((SIEVE_MAX_K + 1) * sizeof(uint32_t));
    if (!primes) exit(1);
    idx_t primes_cnt = 0;

    for (idx_t i = 2; i <= SIEVE_MAX_K; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (primes_cnt < SIEVE_MAX_K) primes[primes_cnt++] = (uint32_t)i;
        }
        for (idx_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            idx_t t = i * (idx_t)p;
            if (t > SIEVE_MAX_K) break;
            spf[t] = p;
            if (p > spf[i]) break;
        }
        omega[i] = omega[i / spf[i]] + 1;
    }
    free(primes);
    spf_limit = SIEVE_MAX_K;
}

/* Storage for the sequence values. */
static idx_t seq_len = 0;
static idx_t seq_cap = 0;
static seq_t *seq_vals = NULL;

/* Append a new term, expanding the array if necessary. */
static void append_seq(seq_t val)
{
    if (seq_len >= seq_cap) {
        idx_t new_cap = (seq_cap == 0) ? 128 : seq_cap * 2;
        seq_t *new_arr = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        seq_vals = new_arr;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = val;
}

/* Compute at least n terms, starting the search from the next unused k. */
static void ensure_terms(idx_t n)
{
    static idx_t next_k = 1;	        /* k to evaluate next */

    while (seq_len < n) {
        if (next_k > spf_limit) exit(1);	/* out of pre‑computed range */

        idx_t a, b;
        if ((next_k & 1) == 0) {	/* k even: k/2 is the even part */
            a = next_k >> 1;
            b = next_k + 1;
        } else {			/* k odd: (k+1)/2 is the even part */
            a = next_k;
            b = (next_k + 1) >> 1;
        }

        /* Count prime factors with multiplicity. */
        uint32_t count = omega[a] + omega[b];
        if (count == 10) {
            seq_t val = (seq_t)next_k * ((seq_t)next_k + 1) / 2;
            append_seq(val);
        }
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;	/* undefined for index 0 – guard value */

    static int init_done = 0;
    if (!init_done) {
        init_sieve();
        init_done = 1;
    }
    ensure_terms(n);
    return seq_vals[n - 1];
}
