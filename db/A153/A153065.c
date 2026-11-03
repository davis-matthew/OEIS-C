/*
 * A153065  Prime numbers such that the sum of any 5 consecutive terms is a
 *          prime.
 * https://oeis.org/A153065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A153065: Prime numbers such that the sum of any 5 consecutive
   terms is a prime.  The function a(n) returns the n‑th such prime
   (n ≥ 1).  The sequence is built iteratively: after the first five
   terms, each new term is the smallest prime greater than the
   predecessor for which the sum of the last five terms (including
   the new one) is prime. */
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

static seq_t *seq          = NULL;   /* storage for the sequence */
static idx_t seq_len       = 0;      /* number of terms stored          */
static size_t seq_cap      = 0;      /* allocated capacity (by count)   */

static void ensure_seq(idx_t n)
/* Make sure the global array contains the first n terms of the sequence. */
{
    while (seq_len < n) {
        /* Allocate initial storage if needed. */
        if (!seq) {
            seq_cap = 16;
            seq = mem_new(seq_t, seq_cap);
            if (!seq) exit(EXIT_FAILURE);
        }
        /* Grow the array *before* writing beyond existing capacity. */
        if (seq_len == seq_cap) {
            seq_cap *= 2;
            seq = mem_resize(seq_t, seq, seq_cap);
            if (!seq) exit(EXIT_FAILURE);
        }

        /* First five terms are known explicitly. */
        if (seq_len < 5) {
            static const seq_t init[5] = {(seq_t)3, (seq_t)5, (seq_t)11,
                                          (seq_t)19, (seq_t)23};
            seq[seq_len] = init[seq_len];
        } else {
            /* Sum of the last four terms. */
            seq_t sum4 = seq[seq_len-1] + seq[seq_len-2]
                       + seq[seq_len-3] + seq[seq_len-4];

            /* Find the next prime that keeps the 5‑term sum prime. */
            seq_t cand = seq[seq_len-1] + (seq_t)2;   /* start with next odd */
            for (;;) {
                if (is_prime_big(cand)) {
                    seq_t total = sum4 + cand;
                    if (is_prime_big(total)) {
                        seq[seq_len] = cand;
                        break;
                    }
                }
                cand += (seq_t)2;
            }
        }
        seq_len++;
    }
}

seq_t a(idx_t n)   /* n ≥ 1 */
{
    ensure_seq(n);
    return seq[n-1];
}
