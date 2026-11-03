/*
 * A088883  Primes which when concatenated with their reverse and incremented
 *          by 2 yield a new prime.
 * https://oeis.org/A088883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 188, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 979 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u128, is_prime_u64, mem, next_prime, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that concat(p, reverse(p)) + 2 is prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility headers (available in the build environment). */
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_seq.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

static seq_t concat_parts(idx_t a, idx_t b)
{
    seq_t part_a = (seq_t)a;
    seq_t part_b = (seq_t)b;
    seq_t pow = 1;
    seq_t tmp = part_b;
    /* pow = 10^(number of decimal digits of b) */
    while (tmp > 0) {
        pow *= 10;
        tmp /= 10;
    }
    return part_a * pow + part_b;
}

/* Static state to memoize results across calls. */
static seq_t *seq_results = NULL;      /* array of sequence values */
static size_t seq_cnt = 0;             /* number of values stored */
static size_t seq_cap = 0;             /* allocated capacity */
static uint64_t current_prime = 1;     /* last prime inspected */

/* Ensure the results array can hold at least 'new_cap' elements. */
static void ensure_seq_cap(size_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    size_t grow = seq_cap ? seq_cap * 2 : 8;      /* start with small size */
    if (grow < new_cap)
        grow = new_cap;
    seq_t *tmp = mem_resize(seq_t, seq_results, grow);
    if (!tmp) exit(1);   /* out of memory */
    seq_results = tmp;
    seq_cap = grow;
}

/* Append a new value to the results array. */
static void append_seq(seq_t val)
{
    ensure_seq_cap(seq_cnt + 1);
    seq_results[seq_cnt++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0 but return 0 to avoid UB */

    /* Generate until we have at least n terms. */
    while (seq_cnt < (size_t)n) {
        /* Find next prime 'current_prime' of the form 6k+1 (or 7). */
        do {
            current_prime = next_prime(current_prime);
        } while (current_prime % 6 != 1);

        /* Form concat(p, reverse(p)) + 2 and test for primality. */
        uint64_t rev_p = reverse_digits_idx(current_prime);
        seq_t conc = concat_parts(current_prime, rev_p);
        conc += (seq_t)2;
        if (isPrimeGeneric(conc))
            append_seq((seq_t)current_prime);
    }
    return seq_results[n - 1];
}
