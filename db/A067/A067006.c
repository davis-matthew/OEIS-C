/*
 * A067006  Smallest number for which the totient is divisible by the n-th
 *          nontotient number, that is, the n-th term of A007617.
 * https://oeis.org/A067006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{x>0 : phi(x) mod A007617(n) == 0}
 * Range:     u8 n <= 16, u16 n <= 1352, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/phi.h"     /* seq_t phi(idx_t m); */

#define LIMIT 2000000

/* ------------------------------------------------------------------ */
/*  Pre‑computation of nontotient numbers (A007617)                   */
/* ------------------------------------------------------------------ */
static uint32_t phi_arr[LIMIT + 1];
static bool is_comp[LIMIT + 1];
static bool totient_exists[LIMIT + 1];
static uint32_t primes_arr[200000];   /* enough for LIMIT up to 2e6 */
static size_t prime_cnt = 0;

static seq_t *nontotients = NULL;
static size_t nt_len   = 0;
static size_t nt_cap   = 0;

/* Linear sieve to compute φ[1..LIMIT] and list of nontotients.       */
static void compute_nontotients(void)
{
    /* --- 1. compute φ ------------------------------------------------- */
    phi_arr[0] = 0;
    phi_arr[1] = 1;
    is_comp[0] = is_comp[1] = true;

    for (size_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes_arr[prime_cnt++] = (uint32_t)i;
            phi_arr[i] = (uint32_t)(i - 1);
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes_arr[j];
            unsigned long long v = (unsigned long long)p * i;
            if (v > LIMIT) break;
            is_comp[(size_t)v] = true;
            if (i % p == 0) {
                phi_arr[(size_t)v] = phi_arr[i] * p;
                break;
            } else {
                phi_arr[(size_t)v] = phi_arr[i] * (p - 1);
            }
        }
    }

    /* --- 2. mark existing totients ---------------------------------- */
    for (size_t i = 1; i <= LIMIT; ++i) {
        uint32_t t = phi_arr[i];
        if (t <= LIMIT) totient_exists[t] = true;
    }

    /* --- 3. collect nontotients ------------------------------------- */
    nt_cap = 250000;                     /* generous initially          */
    nontotients = (seq_t *)malloc(nt_cap * sizeof(seq_t));
    nt_len = 0;

    for (size_t i = 1; i <= LIMIT; ++i) {
        if (!totient_exists[i]) {
            if (nt_len == nt_cap) {
                nt_cap *= 2;
                nontotients = (seq_t *)realloc(nontotients, nt_cap * sizeof(seq_t));
                if (!nontotients) { /* allocation failure – should not happen */
                    exit(EXIT_FAILURE);
                }
            }
            nontotients[nt_len++] = (seq_t)i;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                           */
/* ------------------------------------------------------------------ */
static void ensure_init(void)
{
    static bool initialized = false;
    if (!initialized) {
        compute_nontotients();
        initialized = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for index 0, return 0 as fallback */
    ensure_init();

    if (n > nt_len) return 0;   /* out of bounds, should not happen */

    seq_t t = nontotients[(size_t)(n - 1)];   /* n-th nontotient (A007617) */

    /* Search for the smallest x with φ(x) ≡ 0 mod t. */
    unsigned long long cand = 1;
    while (true) {
        seq_t phi_val = phi((idx_t)cand);
        if (phi_val % t == 0) {
            return (seq_t)cand;
        }
        ++cand;
    }
}
