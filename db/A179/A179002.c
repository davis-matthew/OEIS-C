/*
 * A179002  Primes p such that 2^(2p-1) mod (2p+1) is prime.
 * https://oeis.org/A179002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p such that 2^(2p-1) mod (2p+1) is prime.
 * The compute is done by incremental generation of primes and testing the
 * property; results are memoised, so any call order and n up to about 10 000
 * works within the time limits. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/powmod.h"
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

/* ---- cache of qualifying primes (answers) ---- */
static seq_t *answers     = NULL;   /* array of qualifying primes */
static size_t answers_cnt = 0;      /* number of stored answers */
/* helper to append to the answers cache */
static void append_answer(seq_t val)
{
    if (answers_cnt == 0) {
        answers = mem_new(seq_t, 8);
        if (!answers) exit(1);
        answers_cnt   = 0;
    } else if (answers_cnt >= 8) {
        /* grow on the fly; the initial size 8 is arbitrary and can be
         * expanded whenever the count is a multiple of the capacity */
        size_t new_cap = (answers_cnt < 16 ? 16 : answers_cnt << 1);
        seq_t *tmp = mem_resize(seq_t, answers, new_cap);
        if (!tmp) exit(1);
        answers = tmp;
    }
    answers[answers_cnt++] = val;
}

/* ---- state for incremental prime generation ---- */
static seq_t next_candidate = 2;   /* next odd number to test for primality */
/* The following loop generates only odd numbers after 2, because all
   even numbers >2 are composite. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* offset 1: undefined for n==0 */
    while (answers_cnt < (size_t)n) {       /* generate until we have n answers */

        /* Test whether the current candidate is a prime. */
        if (is_prime_big(next_candidate)) {

            /* Compute m = 2p + 1 in seq_t and the exponent e = 2p - 1 in uint64_t.
               For the expected range of n (up to 10 000) p stays well below
               64 bit limits, so converting to uint64_t is safe. */
            seq_t m = (seq_t)( (next_candidate << 1u) + 1 );
            uint64_t e = (uint64_t)((next_candidate << 1u) - 1);

            /* Compute r = 2^e mod m. The helper mod_pow works for arbitrary
               precision seq_t types and returns a seq_t. */
            seq_t r = powmod((seq_t)2, e, m);

            /* If r is prime, the candidate satisfies the definition of the
               sequence, so store it. */
            if (is_prime_big(r))
                append_answer(next_candidate);
        }

        /* Advance to the next candidate; after 2 we skip even numbers. */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return answers[(size_t)n - 1];
}
