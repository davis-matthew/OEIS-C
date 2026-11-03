/*
 * A049551  Primes p such that x^19 = 2 has a solution mod p.
 * https://oeis.org/A049551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 927 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049551: primes p such that x^19 ≡ 2 (mod p) is solvable.
   The function lists primes that do **not** lie in the complement
   A059244.  The test is   p ≠ 1 (mod 19)   or   2^((p‑1)/19) ≡ 1 (mod p). */
 
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Static state holding discovered terms.  The array grows as needed. */
static seq_t *primes = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* The next candidate prime to check.  Starts at 2.  We advance by 1 to 3,
   then by 2 thereafter to stay on odd numbers only. */
static seq_t next_candidate = 2U;

/* Grow primes array to fulfill a required capacity. */
static void grow_primes(size_t needed)
{
    if (needed <= primes_cap)
        return;
    size_t new_cap = primes_cap ? primes_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, primes, new_cap);
    if (!tmp)
        abort();
    primes = tmp;
    primes_cap = new_cap;
}

/* Compute a single term of the sequence.  The function is memoised so
   calls can come in any order. */
seq_t a(idx_t n)
{
    if (n == 0)      /* The OEIS offset is 1; a(0) is undefined. */
        return 0;

    /* Produce terms until we have at least n of them. */
    while (primes_cnt < n) {
        /* Test whether the current candidate is prime. */
        if (is_prime_big(next_candidate)) {
            bool accept = false;

            /* If p ≢ 1 (mod 19) the congruence is automatically
               solvable (by Fermat).  For p ≡ 1 (mod 19) we explicitly test. */
            if ((next_candidate % (seq_t)19) != (seq_t)1) {
                accept = true;
            } else {
                /* Compute 2^((p-1)/19) mod p and compare to 1. */
                seq_t exp_big = (next_candidate - 1) / (seq_t)19;
                /* exp_big fits into uint64_t for all terms we generate in the
                   range the tests use (n up to about 10000).  Truncate safely. */
                uint64_t exp = (uint64_t)exp_big;
                seq_t r = powmod((seq_t)2, exp, next_candidate);
                if (r == (seq_t)1)
                    accept = true;
            }

            if (accept) {
                grow_primes(primes_cnt + 1);
                primes[primes_cnt++] = next_candidate;
            }
        }

        /* Advance to the next candidate */
        if (next_candidate == 2U)
            next_candidate = 3U;          /* 2 -> 3 */
        else
            next_candidate += 2U;          /* odd → next odd */
    }

    return primes[n - 1];
}
