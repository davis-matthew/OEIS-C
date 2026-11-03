/*
 * A091932  Primes that remain prime when their leading digit in binary
 *          representation is replaced by 0.
 * https://oeis.org/A091932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 978 (2026-11-03)
 * Helpers:   bit_length, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A091932: primes p such that p - 2^⌊log₂p⌋ is also prime.
   a(n) is the nth such prime (n ≥ 1). This implementation searches successive
   primes, stores those that satisfy the condition, and returns the
   requested entry.  It uses the provided is_prime_big() and bitlen_seq_t()
   utilities and caches results across calls. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bit_length.h"
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
    /* Undefined behaviour for n < 1 – return 0 for safety */
    if (n < 1)
        return 0;

    /* Static cache of qualifying primes */
    static seq_t *matches  = NULL;
    static size_t matches_cnt = 0;
    static size_t matches_cap = 0;

    /* Static pointer to the last tested prime
       (initialized to 1 so the first loop starts at 2). */
    static seq_t last_candidate = 1;

    /* Generate matches until we have at least n of them. */
    while (matches_cnt < (size_t)n)
    {
        /* Find the next prime greater than last_candidate. */
        do
        {
            ++last_candidate;
        } while (!is_prime_big(last_candidate));

        /* Compute p - 2^⌊log₂p⌋. */
        unsigned shift = bit_length(last_candidate) - 1; /* ⌊log₂p⌋ */
        seq_t pow2 = ((seq_t)1) << shift;
        seq_t q    = last_candidate - pow2;

        /* If the modified number is also prime, store the candidate. */
        if (is_prime_big(q))
        {
            if (matches_cnt >= matches_cap)
            {
                matches_cap = matches_cap ? matches_cap << 1 : 8;
                seq_t *tmp = mem_resize(seq_t, matches, matches_cap);
                if (!tmp)                  /* allocation failed */
                    return 0;
                matches = tmp;
            }
            matches[matches_cnt++] = last_candidate;
        }
    }

    return matches[n - 1];   /* 1‑based indexing */
}
