/*
 * A038900  Primes p such that 26 is not a square mod p.
 * https://oeis.org/A038900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Primes p for which 26 is NOT a quadratic residue modulo p.
   Computed via Euler's criterion: 26^((p-1)/2) mod p == p-1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored primes */
    static size_t cap   = 0;         /* allocation capacity */
    static size_t sz    = 0;         /* number of terms stored */
    static idx_t cand   = 3;         /* next odd candidate to test */

    while (sz < n) {
        /* skip non‑prime candidates */
        if (!is_prime(cand)) {
            cand += 2;
            continue;
        }

        /* 26 ≡ 0 (mod p) for p = 2 or 13 – these are excluded */
        if (cand == 2 || cand == 13) {
            cand += 2;
            continue;
        }

        seq_t p_val = (seq_t)cand;
        uint64_t exp = (cand - 1) / 2;
        seq_t r = powmod((seq_t)26, exp, p_val);

        if (r == p_val - 1) {        /* 26 is a quadratic non‑residue mod p */
            grow_seq(&cache, &cap, sz + 1);
            cache[sz++] = p_val;
        }
        cand += 2;                   /* next odd candidate */
    }

    return cache[n - 1];
}
