/*
 * A216033  Numbers k such that every prime factor of k^2 + 1 is congruent to
 *          1 (mod 8).
 * https://oeis.org/A216033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k such that every prime divisor of k^2+1
 *            is ≡ 1 (mod 8)
 * Range:     u8 n <= 25, u16 n <= 4651, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts 4, 16, 20, 24, 36, … */

/* The function relies on a small sieve of primes up to an
   upper bound that is sufficient for n up to about 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"   /* declares build_primes_upto */

/* ------------------------------------------------------------------ */
/*  Prime table (once per process)                                    */
/* ------------------------------------------------------------------ */
static uint64_t *primes = NULL;           /* array of primes */
static size_t    primes_cnt = 0;          /* number of primes */
static bool      primes_ready = false;   /* have we built the table? */

static void init_primes(void)
{
    if (primes_ready) return;
    /* A bound of two million primes is more than enough for n ≤ 10 000,
       as the 10 000‑th term is well below k ≈ 4 × 10⁵. */
    idx_t limit = 2000000;
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_ready = true;
}

/* ------------------------------------------------------------------ */
/*  Check whether k satisfies the requirement                         */
/* ------------------------------------------------------------------ */
static bool k_ok(uint64_t k)
{
    /* k is guaranteed to be a multiple of 4 in our search. */
    uint64_t m   = k * k + 1;   /* k^2 + 1  fits in uint64_t for needed k */
    uint64_t tmp = m;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            if ((p & 7ULL) != 1ULL) return false;  /* p ≡ 1 (mod 8) fails */
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {
        /* Remaining prime factor */
        if ((tmp & 7ULL) != 1ULL) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Sequence storage (dynamic array)                                  */
/* ------------------------------------------------------------------ */
static seq_t *seq  = NULL;   /* array of found terms */
static size_t seq_len = 0;   /* number of terms stored */
static size_t seq_cap = 0;   /* allocated capacity */

/* ------------------------------------------------------------------ */
/*  Append a new term to the sequence array                            */
/* ------------------------------------------------------------------ */
static void append_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        seq = new_arr;
    }
    seq[seq_len++] = val;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Guard against zero index; sequence is 1‑based. */
    if (n == 0) return 0;

    /* Lazy initialization on first call. */
    static bool init = false;
    static uint64_t current_k = 0;   /* last tested k */
    if (!init) {
        init_primes();
        init = true;
    }

    /* Generate terms until we have the n‑th one. */
    while (seq_len < (size_t)n) {
        current_k += 4;                     /* all terms are multiples of 4 */
        if (k_ok(current_k))
            append_seq((seq_t)current_k);
    }

    return seq[n - 1];
}
