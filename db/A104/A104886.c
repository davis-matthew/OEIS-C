/*
 * A104886  Sequence A048138, the number of times that a positive integer
 *          occurs as the sum of proper divisors, if Goldbach partitions (two
 *          odd primes, which account for most of the values) are ignored.
 * https://oeis.org/A104886
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of m such that the sum of proper divisors of m
 *            equals n, ignoring m that are the product of two distinct odd
 *            primes (Goldbach partitions)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   compute_spf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/compute_spf.h"
#include "../utils/is_prime.h"

/* Maximum index we support.  Must be >= the maximum n we want to answer. */
#define MAX_S 21000

/* Upper bound on the values of m we sieve.
   Needs to be large enough so that every non‑ignored m producing a sum <= MAX_S
   is included.  1 200 000 is conservatively safe for n up to 10 000. */
#define LIMIT 1200000

static seq_t *cnt   = NULL;    /* counts of m for each proper‑divisor sum */
static uint16_t *spf = NULL;   /* smallest prime factor table */

/* Helper: return true if m = p*q with distinct odd primes p < q. */
static inline bool is_two_odd_prime_product(idx_t m, uint16_t *spf_tbl)
{
    /* Handle small cases quickly */
    if (m < 6) return false;                 /* 2,3,4,5 cannot be such product */

    /* Get smallest prime factor p of m */
    uint16_t p = spf_tbl[m];
    if (!p || p == m) return false;          /* m is prime or 1 */

    idx_t q = m / p;
    if (q == p) return false;                /* distinct primes required */

    /* Both primes must be odd */
    if (!(p & 1U) || !(q & 1U)) return false;

    /* Check that q is prime */
    if (q <= LIMIT) {
        uint16_t spq = spf_tbl[q];
        /* In compute_spf, primes have spf[q] == q or 0; treat both as prime. */
        if (!(spq == 0 || spq == q)) return false;
    } else {
        /* q > LIMIT – use is_prime for safety (very unlikely). */
        if (!is_prime((uint64_t)q)) return false;
    }

    return true;    /* m = p*q with distinct odd primes */
}

/* Initialise the sieve and pre‑compute counts. */
static void init(void)
{
    static bool ready = false;
    if (ready) return;

    /* Allocate counter array (value type seq_t may be an arbitrary‑precision type). */
    cnt = (seq_t *)calloc(MAX_S + 1, sizeof(seq_t));
    if (!cnt) return;   /* allocation failed – silently abort */

    /* Compute smallest prime factors up to LIMIT. */
    spf = compute_spf(LIMIT);
    if (!spf) {
        free(cnt);
        cnt = NULL;
        return;
    }

    /* Sieve for divisor sums (including the number itself). */
    uint32_t *sum = (uint32_t *)calloc(LIMIT + 1, sizeof(uint32_t));
    if (!sum) {
        free(cnt);
        free(spf);
        cnt = NULL;
        spf = NULL;
        return;
    }

    for (idx_t i = 1; i <= LIMIT; ++i) {
        for (idx_t j = i; j <= LIMIT; j += i) {
            sum[j] += (uint32_t)i;
        }
    }

    /* Count valid m for each proper‑divisor sum. */
    for (idx_t m = 2; m <= LIMIT; ++m) {
        uint32_t s = sum[m] - (uint32_t)m;   /* proper‑divisor sum */
        if (s > (uint32_t)MAX_S) continue;  /* out of range we care about */

        if (is_two_odd_prime_product(m, spf)) continue;  /* ignore Goldbach product */

        /* Increment counter for this sum. */
        cnt[s] += (seq_t)1;
    }

    free(sum);
    ready = true;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    init();
    if (!cnt || n > (idx_t)MAX_S) return (seq_t)0;
    return cnt[n];
}
