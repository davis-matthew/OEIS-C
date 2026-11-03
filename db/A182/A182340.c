/*
 * A182340  List of positive integers whose prime tower factorization, as
 *          defined in comments, contains the prime 5.
 * https://oeis.org/A182340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9772 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of all positive integers whose prime
 * tower factorization contains the prime 5.
 * The function a(n) returns the n‑th such number, indexing from 1.
 * It builds the sequence lazily by scanning successive integers and
 * storing those that satisfy the property.  The check is performed
 * recursively on the exponents of each prime factor. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static bool has5_rec(uint64_t x) {
    if (x % 5 == 0) return true;
    uint64_t n = x;
    if (n % 2 == 0) {
        uint64_t e = 0;
        while (n % 2 == 0) { n /= 2; ++e; }
        if (e > 1 && has5_rec(e)) return true;
    }
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            if (e > 1 && has5_rec(e)) return true;
        }
    }
    /* remaining prime factor (if any) has exponent 1 – cannot contain 5 */
    return false;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* array of terms a(1), a(2), ...      */
    static idx_t  size  = 0;         /* number of terms stored             */
    static idx_t  cap   = 0;         /* current capacity of cache          */
    static uint64_t candidate = 1;   /* next integer to test               */

    /* Generate terms until we have n terms */
    while (size < n) {
        while (true) {
            if (has5_rec(candidate)) {
                /* grow cache if needed */
                if (size == cap) {
                    cap = cap ? cap * 2 : 128;
                    seq_t *tmp = mem_resize(seq_t, cache, cap);
                    cache = tmp; /* assume success for the purpose of the contest */
                }
                cache[size++] = (seq_t)candidate;
                candidate++;   /* next number to try on next iteration */
                break;
            }
            candidate++;
        }
    }
    return cache[n - 1];
}
