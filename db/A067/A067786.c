/*
 * A067786  Numbers n such that phi(n) = 2*pi(n) where pi(n) = A000720(n) and
 *          phi(n) = A000010(n).
 * https://oeis.org/A067786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer >0 such that φ(n) = 2·π(n) (offset 1)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   ensure_uint32_cap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ensure_uint32_cap.h"

/* ------------------------------------------------------------------ */
/* Helper: ensure enough room in a dynamic array of uint32_t.          */

/* ------------------------------------------------------------------ */
/* Helper: ensure enough room in a dynamic array of seq_t.           */
/* ------------------------------------------------------------------ */
static void ensure_seq_cap(seq_t **arr, size_t *sz, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t ncap = *cap ? *cap * 2 : 16;
    while (ncap < need) ncap <<= 1;
    seq_t *nptr = (seq_t *)realloc(*arr, ncap * sizeof(seq_t));
    if (!nptr && ncap > 0) exit(EXIT_FAILURE);
    *arr = nptr;
    *cap = ncap;
}

/* ------------------------------------------------------------------ */
/* Prime list and related state.                                       */
/* ------------------------------------------------------------------ */
static uint32_t *prime_list = NULL;
static size_t      prime_cnt  = 0;
static size_t      prime_cap  = 0;

/* ------------------------------------------------------------------ */
/* Sequence terms and related state.                                   */
/* ------------------------------------------------------------------ */
static seq_t *term_list = NULL;   /* term_list[0] is a dummy */
static size_t term_cnt  = 0;
static size_t term_cap  = 0;

/* ------------------------------------------------------------------ */
/* Current maximum checked value and π(n).                              */
/* ------------------------------------------------------------------ */
static idx_t last_checked = 1;
static idx_t pi_cnt       = 0;

/* ------------------------------------------------------------------ */
/* Compute φ(n) using the current prime list.                          */
/* ------------------------------------------------------------------ */
static uint64_t phi_uint64(idx_t n)
{
    uint64_t res = n;
    idx_t t = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > (uint64_t)t) break;
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            res -= res / p;
        }
    }
    if (t > 1) res -= res / (uint64_t)t;
    return res;
}

/* ------------------------------------------------------------------ */
/* Test whether x is prime using the current prime list.               */
/* ------------------------------------------------------------------ */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Append a new prime to the list.                                     */
/* ------------------------------------------------------------------ */
static void add_prime(uint32_t p)
{
    ensure_uint32_cap(&prime_list, &prime_cnt, &prime_cap, prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

/* ------------------------------------------------------------------ */
/* Store a new term of the sequence.                                   */
/* ------------------------------------------------------------------ */
static void add_term(idx_t n)
{
    ensure_seq_cap(&term_list, &term_cnt, &term_cap, term_cnt + 1);
    term_list[term_cnt++] = (seq_t)n;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialise term storage with a dummy at index 0. */
    if (term_cnt == 0) {
        ensure_seq_cap(&term_list, &term_cnt, &term_cap, 1);
        term_list[0] = (seq_t)0;
        term_cnt = 1;
    }

    /* Generate terms until we have at least n terms. */
    while (term_cnt <= n) {
        ++last_checked;

        /* Check and record primes, updating π. */
        if (is_prime(last_checked)) {
            add_prime((uint32_t)last_checked);
            ++pi_cnt;
        }

        /* Compute φ(n) and test the defining equality. */
        uint64_t phi_val = phi_uint64(last_checked);
        uint64_t two_pi  = 2ULL * (uint64_t)pi_cnt;
        if (phi_val == two_pi) {
            add_term(last_checked);
        }
    }

    return term_list[n];
}
