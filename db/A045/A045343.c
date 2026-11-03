/*
 * A045343  Primes congruent to {2, 3} mod 7.
 * https://oeis.org/A045343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p ≡ 2 or 3 (mod 7)
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 881 (2026-11-03)
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

/* Cache of primes found so far.  tbl[0] = 2, the first prime ≡ 2 (mod 7). */
static seq_t *tbl   = NULL;   /* dynamic array of primes */
static size_t tbl_len = 0;     /* number of primes stored */
static size_t tbl_cap = 0;     /* allocated capacity of tbl */

/* Candidate odd number to test next.  Starts at 3. */
static uint64_t cand = 3;

/* Initialise the cache with the first prime. */
static void init_tbl(void) {
    if (!tbl) {
        tbl_cap = 16;
        tbl = mem_new(seq_t, tbl_cap);
        tbl[0] = (seq_t)2;      /* 2 ≡ 2 (mod 7) */
        tbl_len = 1;
    }
}

/* Grow the cache until it contains at least `n` entries. */
static void ensure_up_to(idx_t n) {
    init_tbl();

    while (tbl_len < n) {
        if (is_prime_big((seq_t)cand)) {
            if ((cand % 7 == 2) || (cand % 7 == 3)) {
                if (tbl_len == tbl_cap) {
                    tbl_cap <<= 1;
                    tbl = mem_resize(seq_t, tbl, tbl_cap);
                }
                tbl[tbl_len++] = (seq_t)cand;
            }
        }
        cand += 2;   /* next odd candidate */
    }
}

/* Return the n‑th prime congruent to 2 or 3 modulo 7 (1‑based index). */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined, but harmless */
    ensure_up_to(n);
    return tbl[n - 1];
}
