/*
 * A139317  a(n) = the smallest value of the form n*k + 1, k = positive
 *          integer, that is coprime to all the previous terms of this
 *          sequence.
 * https://oeis.org/A139317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 869 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = the smallest value of the form n*k + 1 (k > 0)
 * that is coprime to all previous terms of the sequence.
 * The sequence is a permutation of the primes.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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
    /* static cache for memoisation */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;   /* number of terms currently stored */
    static size_t seq_cap = 0;   /* capacity of the cache */

    if (n == 0) return 0;   /* undefined for n < 1, return 0 */

    /* grow the cache if we need more terms */
    while (seq_len < (size_t)n) {
        /* increase capacity */
        if (seq_cap == 0) {
            seq_cap = 64;
            seq = mem_new(seq_t, seq_cap);
        } else if (seq_len + 1 > seq_cap) {
            size_t new_cap = seq_cap * 2;
            if (new_cap < seq_len + 1) new_cap = seq_len + 1;
            seq = mem_resize(seq_t, seq, new_cap);
            seq_cap = new_cap;
        }

        /* term number we are computing (1‑based) */
        idx_t idx = seq_len + 1;

        /* search for the smallest k > 0 with the required property */
        seq_t k = 1;
        seq_t candidate;
        while (1) {
            candidate = ((seq_t)idx) * k + (seq_t)1;
            /* candidate must be prime (sequence is a permutation of primes) */
            if (!is_prime_big(candidate)) {
                ++k;
                continue;
            }
            /* check coprimality with all previous terms */
            bool coprime = true;
            for (size_t j = 0; j < seq_len; ++j) {
                if (candidate % seq[j] == 0) {
                    coprime = false;
                    break;
                }
            }
            if (coprime) {
                seq[seq_len++] = candidate;
                break;
            }
            ++k;
        }
    }

    return seq[n - 1];
}
