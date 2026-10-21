/*
 * A068422  Numbers k such that k-phi(k) divides sigma(k).
 * https://oeis.org/A068422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 7471, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068422: numbers k such that k-φ(k) divides σ(k).
   This routine enumerates integers k and tests the condition
   by computing φ(k) and σ(k) using a smallest‑prime‑factor sieve.
   It memoises the results so any call with n ≥ 1 returns the
   n‑th term in time linear in the value of the term. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* smallest prime factor table (uint32_t *) for numbers up to spf_limit */
static uint32_t *spf = NULL;
static size_t   spf_limit = 0;

/* array of found terms */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* next integer to test */
static idx_t cur_k = 2;

static void build_spf(size_t limit)
{
    free(spf);
    spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
    for (size_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
    spf_limit = limit;
}

static seq_t phi_from_spf(uint32_t k)
{
    seq_t res = (seq_t)k;
    uint32_t t = k;
    while (t > 1) {
        uint32_t p = spf[t];
        res = res / p * (p - 1);
        do { t /= p; } while (t % p == 0);
    }
    return res;
}

static seq_t sigma_from_spf(uint32_t k)
{
    seq_t res = 1;
    uint32_t t = k;
    while (t > 1) {
        uint32_t p = spf[t];
        int e = 0;
        while (t % p == 0) { t /= p; ++e; }
        __int128 powp = 1;
        for (int i = 0; i < e + 1; ++i) powp *= p;
        __int128 den  = (__int128)(p - 1);
        __int128 term = (powp - 1) / den;
        res = (seq_t)((__int128)res * term);
    }
    return res;
}

static void maybe_resize_sequence(void)
{
    if (seq_cap == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    } else if (seq_len >= seq_cap) {
        seq_cap *= 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (seq_len >= (size_t)n) return seq[n-1];

    if (spf == NULL) build_spf(2000000);

    while (seq_len < (size_t)n) {
        if ((size_t)cur_k > spf_limit) {
            size_t new_limit = spf_limit * 2;
            while ((size_t)cur_k > new_limit) new_limit *= 2;
            build_spf(new_limit);
        }
        uint32_t k = (uint32_t)cur_k;
        seq_t phi  = phi_from_spf(k);
        seq_t sigma= sigma_from_spf(k);
        seq_t d = (seq_t)k - phi;
        if (d != 0 && sigma % d == 0) {
            maybe_resize_sequence();
            seq[seq_len++] = (seq_t)k;
        }
        ++cur_k;
    }
    return seq[n-1];
}
