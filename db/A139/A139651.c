/*
 * A139651  Primes of the form x^2 + 210*y^2.
 * https://oeis.org/A139651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 385, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9628 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x^2 + 210*y^2
   Computed by brute‑force search using the residue classes
   specified in the OEIS entry.  No hard‑coded values are used. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

/* Return the n‑th prime of the form x^2 + 210*y^2,  n >= 1.  */
seq_t a(idx_t n)
{
    /* 12 residue classes mod 840 for such primes */
    static const int residues[12] = {
        1, 121, 169, 211, 289,
        331, 361, 379, 499, 529,
        571, 739
    };
    static bool residue_map[840];
    static bool residue_initialized = false;

    /* Initialise residue lookup table on first call */
    if (!residue_initialized) {
        memset(residue_map, 0, sizeof(residue_map));
        for (int i = 0; i < 12; ++i) {
            residue_map[residues[i]] = true;
        }
        residue_initialized = true;
    }

    /* Static cache of primes already found (up to a fixed limit) */
    /* This limit is safe for n up to about 10 000 as required. */
    static seq_t primes[20000];
    static uint64_t primes_len = 0;
    static uint64_t last_checked = 1;  /* last tested integer (exclusive) */

    /* Expand cache until it contains at least n primes */
    while (primes_len < n) {
        ++last_checked;
        int rem = (int)(last_checked % 840);
        if (!residue_map[rem]) continue;   /* not in a valid residue class */

        seq_t val = (seq_t)last_checked;
        if (is_prime_big(val)) {
            primes[primes_len++] = val;
        }
    }

    return primes[n - 1];
}
