/*
 * A172624  Numbers k such that 2*A000005(k) = 3*A000005(k+1).
 * https://oeis.org/A172624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where 2 * tau(k) == 3 * tau(k+1), with tau being the
 *            number of positive divisors
 * Range:     u8 n <= 12, u16 n <= 2585, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the nth element of this sequence,
   with indexing starting at 1. */

#include <stdint.h>
#include <stdlib.h>

/* ---- Global static data ------------------------------------------- */
static size_t sieve_limit   = 0;     /* highest number with known tau      */
static size_t processed_upto = 0;    /* last k that was tested            */
static uint32_t *spf          = NULL;     /* smallest prime factor             */
static seq_t   *tau_arr        = NULL;     /* divisor counts                    */
static size_t  seq_cap  = 0;    /* capacity of seq_arr                */
static seq_t   *seq_arr  = NULL;   /* collected sequence values          */
static size_t  seq_len  = 0;    /* number of terms stored             */

/* ---- Helper to grow the sequence array ---------------------------- */
static void push_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t newcap = seq_cap ? seq_cap * 2 : 16;
        seq_arr = (seq_t *)realloc(seq_arr, newcap * sizeof(seq_t));
        seq_cap = newcap;
    }
    seq_arr[seq_len++] = val;
}

/* ---- Compute SPF and tau up to new_limit (rebuild from scratch) --- */
static void extend_sieve(size_t new_limit)
{
    if (new_limit <= sieve_limit) return;

    /* Allocate fresh arrays and zero them */
    uint32_t *new_spf = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
    seq_t   *new_tau  = (seq_t   *)calloc(new_limit + 1, sizeof(seq_t));

    /* Smallest prime factor sieve */
    for (size_t i = 2; i <= new_limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i <= new_limit / i) {
                for (size_t j = i * i; j <= new_limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    /* Divisor count computation */
    new_tau[1] = 1;
    for (size_t i = 2; i <= new_limit; ++i) {
        size_t m = i;
        seq_t  tau = 1;
        while (m > 1) {
            uint32_t p   = new_spf[m];
            uint32_t cnt = 0;
            while (m % p == 0) { m /= p; ++cnt; }
            tau *= (cnt + 1);
        }
        new_tau[i] = tau;
    }

    /* Collect terms satisfying 2*tau(k) == 3*tau(k+1) */
    for (size_t k = processed_upto + 1; k <= new_limit - 1; ++k) {
        if (new_tau[k] * 2 == new_tau[k + 1] * 3)
            push_seq((seq_t)k);
    }

    /* Free old arrays and install the new ones */
    free(spf);
    free(tau_arr);
    spf   = new_spf;
    tau_arr = new_tau;

    processed_upto = new_limit - 1;
    sieve_limit    = new_limit;
}

/* ---- Ensure we have at least n terms -------------------------------- */
static void ensure_terms(idx_t n)
{
    while ((size_t)n > seq_len) {
        size_t new_limit = sieve_limit ? sieve_limit * 2 : 1000;
        size_t needed   = (size_t)n * 10 + 1000;   /* heuristic */
        if (new_limit < needed) new_limit = needed;
        extend_sieve(new_limit);
    }
}

/* ---- Public API ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 0, but safe */
    ensure_terms(n);
    return seq_arr[n - 1];
}
