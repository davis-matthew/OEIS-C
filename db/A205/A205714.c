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
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

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
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
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
