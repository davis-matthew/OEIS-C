/*
 * A133835  Slowest increasing sequence of primes such that two neighbor terms
 *          share at least two digits (counted with multiplicity).
 * https://oeis.org/A133835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th term of the slowest increasing sequence of
 *            primes such that adjacent terms share at least two digits
 *            (counted with multiplicity). This implementation iteratively
 *            builds the sequence, caching all computed terms
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   digit_frequencies, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility functions provided by the build system. */
#include "../utils/digit_frequencies.h"
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

/* Return true iff a and b share at least two digits (with multiplicity). */
static bool shares_two_digits(seq_t a, seq_t b)
{
    unsigned int fa[10] = {0};
    unsigned int fb[10] = {0};
    digit_frequencies(a, fa);
    digit_frequencies(b, fb);
    unsigned int overlap = 0;
    for (int d = 0; d < 10; ++d) {
        overlap += fa[d] < fb[d] ? fa[d] : fb[d];
    }
    return overlap >= 2;
}

/* Dynamic storage of the sequence. */
static seq_t *seq_buffer   = NULL;
static size_t seq_capacity = 0;
static size_t seq_length   = 0;

/* Grow the buffer enough to hold needed elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_capacity) return;
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
    while (new_cap < needed) new_cap *= 2U;
    seq_buffer = mem_resize(seq_t, seq_buffer, new_cap);
    seq_capacity = new_cap;
}

/* Main entry point.  n is 1‑based; a(1) = 11. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset < 1 is irrelevant */
    while (seq_length < (size_t)n) {
        seq_t next_prime;
        if (seq_length == 0) {
            /* first term of the sequence */
            next_prime = 11;
        } else {
            seq_t prev = seq_buffer[seq_length - 1];
            seq_t cand = prev + 1;
            /* keep only odd candidates (prev is odd). */
            if (cand % 2 == 0) cand += 1;
            /* search for the next prime that shares at least two digits
               with the previous prime. */
            while (true) {
                if (is_prime_big(cand) && shares_two_digits(prev, cand))
                    break;
                cand += 2;           /* stay odd */
            }
            next_prime = cand;
        }
        ensure_capacity(seq_length + 1);
        seq_buffer[seq_length++] = next_prime;
    }
    return seq_buffer[n - 1];
}
