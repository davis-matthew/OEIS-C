/*
 * A108899  Numbers k such that 11*k + 2357 is prime.
 * https://oeis.org/A108899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer k such that 11*k + 2357 is prime
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit integers */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static bool isPrime64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    if (n % 5 == 0) return n == 5;

    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    const unsigned int nbases = 7;
    for (unsigned int i = 0; i < nbases; ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;

        /* Compute a^d mod n */
        uint64_t x = 1;
        uint64_t base = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = mod_mul(x, base, n);
            base = mod_mul(base, base, n);
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Memoisation table for already computed terms */
static seq_t *table = NULL;
static size_t tbl_len = 0;
static size_t tbl_cap = 0;

/* Next k to test; start from 0 */
static uint64_t next_k = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset is 1; undefined behaviour for n=0 */

    /* initialise table if it does not yet exist */
    if (tbl_cap == 0) {
        tbl_cap = 16;
        table = (seq_t *)malloc(tbl_cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n of them */
    while (tbl_len < (size_t)n) {
        /* search for the next k producing a prime */
        uint64_t k = next_k;
        while (true) {
            uint64_t value = 11ULL * k + 2357ULL;
            if (isPrime64(value)) break;
            ++k;
        }

        /* store the found k */
        if (tbl_len == tbl_cap) {
            tbl_cap <<= 1;
            table = (seq_t *)realloc(table, tbl_cap * sizeof(seq_t));
        }
        table[tbl_len++] = (seq_t)k;

        /* update the start for the next search */
        next_k = k + 1;
    }

    return table[n - 1];
}
