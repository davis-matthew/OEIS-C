/*
 * A062373  Ratio of totient to Carmichael's lambda function is 2.
 * https://oeis.org/A062373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 9979, u32 n <= 9979, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062373: k such that φ(k)/λ(k) = 2.
   The function silently builds the list by scanning
   natural numbers and testing each candidate; the results
   are cached in a static array so subsequent calls are O(1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */

static uint64_t *spf = NULL;          /* smallest prime factor array   */
static uint64_t  spf_limit = 0;        /* current size of the sieve     */

/* Extend the sieve to cover at least n */
static void ensure_spf(uint64_t n)
{
    if (n <= spf_limit) return;
    uint64_t new_limit = spf_limit * 2 + 1;
    if (new_limit < n) new_limit = n;
    uint64_t *tmp = (uint64_t*)calloc(new_limit + 1, sizeof(uint64_t));
    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (!tmp[i]) {                      /* i is prime            */
            tmp[i] = i;
            if (i * i <= new_limit) {
                for (uint64_t j = i * i; j <= new_limit; j += i)
                    if (!tmp[j]) tmp[j] = i;
            }
        }
    }
    tmp[1] = 1;
    free(spf);                              /* free old array          */
    spf = tmp;
    spf_limit = new_limit;
}

/* Test whether k satisfies φ(k)/λ(k) = 2 */
static bool good_candidate(uint64_t k)
{
    if (k < 8) return false;                /* smallest term is 8       */
    seq_t phi = 1;                          /* Euler totient             */
    seq_t lam = 1;                          /* Carmichael lambda        */
    uint64_t t = k;
    while (t > 1) {
        uint64_t p = spf[t];
        uint64_t e = 0;
        while (t % p == 0) { t /= p; ++e; }

        /* φ(p^e) = p^(e-1) * (p-1) */
        seq_t powp = 1;
        for (uint64_t i = 1; i < e; ++i) powp *= (seq_t)p;
        seq_t phi_pe = powp * (seq_t)(p - 1);
        phi *= phi_pe;

        /* λ(p^e) */
        seq_t lambda_pe;
        if (p == 2) {
            if (e == 1)          lambda_pe = 1;
            else if (e == 2)     lambda_pe = 2;
            else {
                seq_t pow2 = 1;
                for (uint64_t i = 0; i < e - 2; ++i) pow2 *= 2;
                lambda_pe = pow2;
            }
        } else {
            lambda_pe = phi_pe;          /* odd prime powers        */
        }

        /* Combine coprime components */
        seq_t g = gcd(lam, lambda_pe);
        lam = lam / g * lambda_pe;
    }
    return (phi == (lam * 2));
}

static seq_t *seq_arr = NULL;             /* cached sequence values   */
static size_t seq_len = 0;                /* number of stored elements */
static size_t seq_cap = 0;                /* allocated capacity       */

/* Grow the cache if needed */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_arr = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1; undefined for 0 */

    static uint64_t last_checked = 0;         /* highest k already examined */

    /* Scan forward until we have at least n terms */
    while (seq_len < (size_t)n) {
        ++last_checked;
        if (last_checked > spf_limit) ensure_spf(last_checked);
        if (good_candidate(last_checked)) {
            ensure_seq_cap(seq_len + 1);
            seq_arr[seq_len++] = (seq_t)last_checked;
        }
    }
    return seq_arr[n - 1];
}
