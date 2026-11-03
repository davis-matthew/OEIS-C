/*
 * A095680  13, together with primes of the form 10^k + 13 for some k >= 0.
 * https://oeis.org/A095680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A095680: 13 followed by primes of the form 10^k + 13 (k ≥ 2). */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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
    /* Static storage for the terms of the sequence. */
    static seq_t *terms   = NULL;
    static size_t size    = 0;
    static size_t cap     = 0;

    /* Static power of ten: 10^2 at start (k = 2). */
    static seq_t pow10     = (seq_t)100;   /* 10^2 */

    /* Initialise on first call. */
    if (size == 0) {
        cap    = 16;
        terms  = mem_new(seq_t, cap);
        terms[0] = (seq_t)13;   /* a(1) = 13 */
        size = 1;
    }

    /* Generate terms until we have at least n of them. */
    while (size < (size_t)n) {
        /* Candidate = 10^k + 13.  pow10 already holds 10^k. */
        seq_t cand = pow10 + (seq_t)13;

        bool prime = false;

        /* Determine if cand fits in 64 bits for a faster test. */
        /* For unsigned types overflow wraps, so we check bounds
         * by comparing against the maximum value minus 13. */
        if (pow10 <= ((seq_t)((uint64_t)UINT64_MAX) - (seq_t)13)) {
            prime = is_prime_u64((uint64_t)cand);
        } else {
            prime = is_prime_big(cand);
        }

        if (prime) {
            if (size == cap) {
                cap *= 2;
                terms = mem_resize(seq_t, terms, cap);
            }
            terms[size++] = cand;
        }

        /* Advance to 10^(k+1).  Detect overflow for unsigned types. */
        seq_t prev = pow10;
        pow10 = pow10 * (seq_t)10;
        /* If multiplication wrapped, we cannot generate larger terms. */
        if (pow10 < prev) {
            /* No further powers of ten can be represented; stop. */
            break;
        }
    }

    return terms[(size_t)n - 1];
}
