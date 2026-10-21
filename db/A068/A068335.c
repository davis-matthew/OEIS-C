/*
 * A068335  Numbers k such that 3^tau(k) <= k < 2*3^tau(k).
 * https://oeis.org/A068335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 133, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068335: Numbers k such that 3^τ(k) ≤ k < 2·3^τ(k) */
/* The implementation uses a simple prime sieve and trial‑division
   factoring.  Memoised terms are stored so that calls to a(n) with
   arbitrary indices are efficient. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

/* ---------- Prime sieve (generated once) ---------- */
static uint32_t *primes = NULL;  /* array of primes */
static size_t primes_len = 0;

/* Sieve primes up to LIMIT.  LIMIT should be large enough for the
   expected range of k.  5 000 000 covers all k needed for n ≤ 10 000. */
static void init_primes(void)
{
    if (primes != NULL) return;               /* already initialised */

    const uint32_t LIMIT = 5000000;            /* adjust if needed */
    primes = (uint32_t *)malloc(LIMIT * sizeof(uint32_t));
    if (!primes) return;                      /* allocation failure */

    bool *is_composite = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!is_composite) { free(primes); return; }

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[primes_len++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }
    free(is_composite);
}

/* ---------- Storage of found terms ---------- */
static seq_t  *terms = NULL;
static size_t  terms_len = 0;
static size_t  terms_cap = 0;

static void add_term(idx_t k)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return;            /* allocation failure – abort silently */
        terms = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = (seq_t)k;
}

/* ---------- τ(n) via trial division using the prime list ---------- */
static idx_t tau_of(idx_t n)
{
    idx_t temp = n;
    idx_t tau  = 1;
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > temp) break;
        if (temp % p == 0) {
            size_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            tau *= (cnt + 1);
        }
    }
    if (temp > 1) tau *= 2;      /* remaining prime factor */
    return tau;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined offset, but safe to return 0 */

    init_primes();

    idx_t k = 1;
    if (terms_len > 0)          /* continue from where we left off */
        k = (idx_t)terms[terms_len - 1] + 1;

    while (terms_len < (size_t)n) {
        idx_t tau = tau_of(k);
        seq_t pow3 = big_pow((seq_t)3, (seq_t)tau);   /* 3^τ(k) */
        seq_t kseq = (seq_t)k;
        seq_t pow3_2 = mul_small(pow3, 2u);           /* 2*3^τ(k) */

        if (pow3 <= kseq && kseq < pow3_2)
            add_term(k);

        ++k;
    }
    return terms[n - 1];
}
