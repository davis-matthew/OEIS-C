/*
 * A205714  Prime(A205712(n)), the n-th number s(k) such that 9 divides
 *          s(k)-s(j) for some j<k, where s(j)=prime(j).
 * https://oeis.org/A205714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prime(A205712(n)) Implementation: Generate primes in
 *            order. For each prime p, let r = p mod 9. Count how many
 *            previous primes share the same residue r; if that count = k,
 *            output p exactly k times (duplicates allowed). This produces the
 *            required sequence
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
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

/* Static state for memoization */
static seq_t *seq_cache = NULL;          /* terms of the sequence              */
static size_t seq_len  = 0;              /* number of terms currently cached   */
static size_t seq_cap  = 0;              /* capacity of seq_cache              */

/* Count of previous primes for each residue mod 9 (0..8) */
static size_t residue_count[9] = {0};

/* Next integer candidate to test for primality */
static uint64_t next_candidate = 2;

/* Ensure seq_cache has space for at least 'needed' elements */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 64;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    if (!tmp)
        abort(); // allocation failed
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* Main function required by the library framework */
seq_t a(idx_t n)
{
    if (n <= seq_len)
        return seq_cache[n - 1];

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        /* find next prime */
        seq_t p;
        for (;;) {
            p = (seq_t)next_candidate;
            next_candidate++;
            if (isPrimeGeneric(p))
                break;
        }

        /* number of previous primes with same residue mod 9 */
        uint64_t r = (uint64_t)(p % (seq_t)9);
        size_t cnt = residue_count[r];

        /* output p 'cnt' times */
        for (size_t i = 0; i < cnt; ++i) {
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = p;
        }

        /* record this prime for future counts */
        residue_count[r]++;

        /* the loop continues if more terms are needed */
    }

    return seq_cache[n - 1];
}
