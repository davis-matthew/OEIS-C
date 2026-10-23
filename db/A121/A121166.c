/*
 * A121166  Integers m that divide A120492(m) = 1 + Sum_{k=1..m}
 *          prime(k)^(m-1).
 * https://oeis.org/A121166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that m divides 1 + Σ_{k=1..m}
 *            prime(k)^(m-1) (OEIS A121166)
 * Range:     u8 n <= 24, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* bool is_prime(idx_t x)      */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t b, uint64_t e, seq_t m) */

static void ensure_primes_up_to(idx_t need);
static void ensure_vals_capacity(size_t need);

static uint64_t *primes = NULL;           /* list of primes */
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t next_candidate = 3;        /* next odd to test for primality */

static seq_t *vals = NULL;                /* found sequence values */
static size_t val_cnt = 0;
static size_t val_cap = 0;
static idx_t current_m = 1;               /* next candidate m to test */

/* ------------------------------------------------------------------ */
/* Grow the primes array to at least 'need' elements by trial division */
static void ensure_primes_up_to(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;

    /* initialise with the first prime if empty */
    if (prime_cnt == 0) {
        bare_small_array_increase: ; /* label to avoid double init */
        if (prime_cap == 0) {
            prime_cap = 16;
            primes = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        }
        primes[prime_cnt++] = 2;
    }

    while (prime_cnt < (size_t)need) {
        uint64_t cand = next_candidate;
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            /* stop when p*p > cand, use 128-bit to avoid overflow */
            if ((__uint128_t)p * p > cand)
                break;
            if (cand % p == 0) {
                ok = false;
                break;
            }
        }
        if (ok) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                primes = (uint64_t*)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_cnt++] = cand;
        }
        next_candidate += 2ULL;          /* advance to next odd */
    }
}

/* ------------------------------------------------------------------ */
/* Grow the values array to at least 'need' elements */
static void ensure_vals_capacity(size_t need)
{
    if (val_cap >= need)
        return;
    val_cap = val_cap ? val_cap * 2 : 16;
    vals = (seq_t*)realloc(vals, val_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/* Core function requested by the build */
seq_t a(idx_t n)
{
    /* Build primes up to the maximum m we might need for the current
       requested n.  We will extend incrementally as more terms are
       needed. */
    while (val_cnt < n) {
        /* For each new m, check if it belongs to the sequence. */
        if (current_m == 1) {            /* 1 is always a term */
            ensure_vals_capacity(val_cnt + 1);
            vals[val_cnt++] = (seq_t)1;
        } else if (is_prime(current_m)) {   /* all primes are in the sequence */
            ensure_vals_capacity(val_cnt + 1);
            vals[val_cnt++] = (seq_t)current_m;
        } else {
            /* Need at least current_m primes for the sum. */
            ensure_primes_up_to(current_m);

            seq_t sum = 0;
            seq_t m_mod = (seq_t)current_m;
            for (idx_t k = 1; k <= current_m; ++k) {
                seq_t prime_k = (seq_t)primes[k - 1];
                seq_t term   = mod_pow(prime_k, current_m - 1, m_mod);
                sum += term;
                sum %= m_mod;              /* keep it small */
            }
            seq_t test = (sum + 1) % m_mod;
            if (test == 0) {
                ensure_vals_capacity(val_cnt + 1);
                vals[val_cnt++] = (seq_t)current_m;
            }
        }
        ++current_m;
    }
    return vals[n - 1];
}
