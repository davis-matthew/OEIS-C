/*
 * A038900  Primes p such that 26 is not a square mod p.
 * https://oeis.org/A038900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq.h"

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
        seq_t r = mod_pow((seq_t)26, exp, p_val);

        if (r == p_val - 1) {        /* 26 is a quadratic non‑residue mod p */
            grow_seq(&cache, &cap, sz + 1);
            cache[sz++] = p_val;
        }
        cand += 2;                   /* next odd candidate */
    }

    return cache[n - 1];
}
