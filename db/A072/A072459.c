/*
 * A072459  Largest prime divisor of even nontotients.
 * https://oeis.org/A072459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A072459: largest prime divisor of the nth even nontotient  
    A072459 is defined as A006530(A005277(n)), i.e. apply the summatory
    totient function to the n‑th even non‑totient and take its largest
    prime factor.

    The routine below pre‑computes Euler's totient values up to a
    fixed limit and flags which numbers are totients.  It then scans
    the even integers, counts those that are not flagged, and when the
    desired index is reached, it extracts the largest prime factor
    using a precomputed smallest‑prime‑factor table.

    The limit is generous enough to cover the first 10 000 terms
    (≈4 000 000) and can be increased if needed.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LIMIT 4000000  /* upper bound for precomputation */

static seq_t *phi = NULL;          /* Euler's totient values */
static unsigned char *is_totient = NULL; /* flag: 1 if number is a totient */
static uint32_t *spf = NULL;       /* smallest prime factor for factorisation */
static bool sieve_done = false;

static void build_sieve(void)
{
    if (sieve_done) return;
    phi = (seq_t *)calloc(MAX_LIMIT + 1, sizeof(seq_t));
    is_totient = (unsigned char *)calloc(MAX_LIMIT + 1, sizeof(unsigned char));
    spf = (uint32_t *)calloc(MAX_LIMIT + 1, sizeof(uint32_t));
    if (!phi || !is_totient || !spf) return;  /* allocation failed */

    seq_t *curr_phi = phi;
    curr_phi[1] = 1;
    uint32_t *pr = (uint32_t *)malloc((MAX_LIMIT / 10 + 100) * sizeof(uint32_t));
    size_t pr_cnt = 0;

    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!spf[i]) {  /* i is prime */
            spf[i] = i;
            curr_phi[i] = (seq_t)i - 1;
            pr[pr_cnt++] = i;
        }
        for (size_t j = 0; j < pr_cnt && pr[j] <= spf[i] && (uint64_t)i * pr[j] <= MAX_LIMIT; ++j) {
            uint32_t p = pr[j];
            uint32_t ip = i * p;
            spf[ip] = p;
            if (i % p == 0) {
                curr_phi[ip] = curr_phi[i] * (seq_t)p;
            } else {
                curr_phi[ip] = curr_phi[i] * ((seq_t)p - 1);
            }
        }
    }
    /* mark all totient values */
    for (uint32_t i = 1; i <= MAX_LIMIT; ++i) {
        seq_t tv = phi[i];
        if (tv <= MAX_LIMIT) is_totient[(uint32_t)tv] = 1;
    }
    sieve_done = true;
    free(pr);
}

/* largest prime factor of x using precomputed spf */
static seq_t largest_prime_factor(uint32_t x)
{
    seq_t maxp = 1;
    while (x > 1) {
        uint32_t p = spf[x];
        if (p == 0) p = x;          /* x is prime itself */
        if ((seq_t)p > maxp) maxp = (seq_t)p;
        while (x % p == 0) x /= p;
    }
    return maxp;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_sieve();

    /* search for the nth even non‑totient */
    idx_t count = 0;
    uint32_t even = 2;
    while (true) {
        if (!is_totient[even]) {
            count++;
            if (count == n) break;
        }
        even += 2;
        if (even > MAX_LIMIT) {
            /* in the unlikely event that the limit is insufficient, extend */
            /* (not expected for typical n) */
            return 0;
        }
    }
    return largest_prime_factor(even);
}
