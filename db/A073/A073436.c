/*
 * A073436  Smallest k such that k mod pi(k) = n.
 * https://oeis.org/A073436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 6471, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9878 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073436:  a(n) = smallest k such that  k mod π(k) = n */
/* (π(k) = number of primes ≤ k)                                         */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --------------------------------------------------------------- */
static void ensure_ans_cap(size_t *cap, seq_t **arr, size_t need)
{
    if (*cap > need) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 64;
    while (new_cap <= need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    for (size_t i = *cap; i < new_cap; ++i) tmp[i] = 0;
    *arr = tmp;
    *cap = new_cap;
}

static void ensure_prime_cap(size_t *cap, uint64_t **arr, size_t need)
{
    if (*cap > need) return;
    size_t new_cap = (*cap) ? (*cap) << 1 : 64;
    while (new_cap <= need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(uint64_t));
    *arr = tmp;
    *cap = new_cap;
}

/* --------------------------------------------------------------- */
/*   Helpers for dynamic tables (used only inside a())              */

/* --------------------------------------------------------------- */
/*   Main sequence function                                       */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Persistent static state */
    static seq_t *answers = NULL;
    static size_t ans_cap = 0;
    static uint64_t *primes = NULL;
    static size_t prc_cap = 0, prc_len = 0;
    static uint64_t current_k = 1;          /* last processed k              */
    static uint64_t current_pi = 0;         /* π(k) for current_k            */

    /* Ensure answers table large enough for n */
    if (!answers) {
        answers = mem_new(seq_t, 64);
        ans_cap = 64;
        for (size_t i = 0; i < ans_cap; ++i) answers[i] = 0;
    }
    ensure_ans_cap(&ans_cap, &answers, (size_t)n);

    /* Return already known value */
    if (answers[(size_t)n] != 0) return answers[(size_t)n];

    /* Ensure primes table exists */
    if (!primes) {
        primes = (uint64_t *)malloc(64 * sizeof(uint64_t));
        prc_cap = 64;
        prc_len = 0;
    }

    /* Main loop: extend k until we find a(n) */
    for (;;) {
        ++current_k;

        /* Prime test for current_k using stored primes */
        bool is_prime = true;
        if (current_k >= 2) {
            for (size_t i = 0; i < prc_len; ++i) {
                uint64_t p = primes[i];
                if (p > current_k / p) break;     /* p*p > current_k */
                if (current_k % p == 0) { is_prime = false; break; }
            }
            if (is_prime) {
                if (prc_len == prc_cap) {
                    ensure_prime_cap(&prc_cap, &primes, prc_cap); /* double */
                }
                primes[prc_len++] = current_k;
                ++current_pi;
            }
        }

        /* π(k)=0 only for k<2; skip those */
        if (current_pi == 0) continue;

        uint64_t rem = current_k % current_pi;

        /* Store answer if this is the first k giving this remainder */
        if (rem <= (uint64_t)n && answers[(size_t)rem] == 0) {
            answers[(size_t)rem] = (seq_t)current_k;
            if (rem == (uint64_t)n) return answers[(size_t)n];
        } else if (rem == (uint64_t)n) {
            return answers[(size_t)n];
        }
    }
}
