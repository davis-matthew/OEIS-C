/*
 * A140039  Primes of the form 28x^2+28xy+73y^2.
 * https://oeis.org/A140039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Primes represented by 28x^2 + 28xy + 73y^2. The primes are
 *            exactly those congruent to the residues
 *            {73,145,241,409,481,601,745,769,937,985,1249,1273,1657,1777,1825}
 *            (mod 1848). This routine generates these primes in ascending
 *            order using that residue representation and a primality test
 * Range:     u8 n <= 2, u16 n <= 211, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9265 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Cache of already found primes */
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cap = 0;

    /* Generation state */
    static uint64_t current_k = 0;     /* polynomial count */
    static size_t residue_idx = 0;     /* next residue to test */

    static const uint16_t residues[15] = {
        73, 145, 241, 409, 481, 601,
        745, 769, 937, 985, 1249, 1273,
        1657, 1777, 1825
    };

    if (n == 0) return 0;         /* undefined for offset 0 – harmless */

    while (cached < (size_t)n) {
        /* Compute the candidate prime for this (k, residue) pair */
        uint64_t pk = 1848ULL * current_k + residues[residue_idx];
        seq_t cand = (seq_t)pk;

        /* Advance to the next residue for the following iteration */
        ++residue_idx;
        if (residue_idx == 15) {   /* finished current k, move to next k */
            residue_idx = 0;
            ++current_k;
        }

        /* Test for primality and store if prime */
        if (cand >= 2 && is_prime_big(cand)) {
            if (cached >= cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;   /* out of memory – unlikely */
                cache = tmp;
                cap = new_cap;
            }
            cache[cached++] = cand;
            if (cached >= (size_t)n) break;
        }
    }
    return cache[n - 1];
}
