/*
 * A040147  Primes p such that x^4 = 20 has a solution mod p.
 * https://oeis.org/A040147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 976 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Tests whether 20 is a fourth power residue modulo p;
   returns the nth prime p such that ∃x with x^4 ≡ 20 (mod p). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
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

seq_t a(idx_t n) {
    static seq_t *primes = NULL;   /* cached primes that satisfy the condition */
    static size_t len   = 0;        /* number of stored primes */
    static size_t cap   = 0;        /* allocated capacity */
    static uint64_t last = 2;       /* last integer checked for primality */

    while (len < (size_t)n) {          /* generate new terms until we have enough */
        /* find next prime ≥ last */
        uint64_t p = last;
        while (!is_prime((idx_t)p)) ++p;

        bool good = false;
        if (p == 2 || p == 5) {
            good = true;              /* 20 is 0 mod p → trivial solution */
        } else {
            /* compute gcd(4, p-1). For odd primes, it's 4 if p≡1 (mod 4), else 2 */
            uint64_t g = (p % 4 == 1) ? 4 : 2;
            uint64_t exp = (p - 1) / g;
            /* a = 20 mod p (which is non‑zero here) */
            seq_t a = (seq_t)(20 % p);
            seq_t r = powmod(a, exp, (seq_t)p);
            good = (r == (seq_t)1);
        }

        if (good) {
            if (len == cap) {
                grow_seq(&primes, &cap, len + 1);
                /* grow_seq might reallocate; ensure primes isn't NULL now */
                if (!primes) return 0;
            }
            primes[len++] = (seq_t)p;
        }

        last = p + 1;          /* start searching from the next number */
    }

    return primes[n - 1];
}
