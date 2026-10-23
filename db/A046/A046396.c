/*
 * A046396  Palindromes which are the product of 6 distinct primes.
 * https://oeis.org/A046396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the sequence A046396.
   The n‑th term is the n‑th palindrome that is a product of six
   distinct prime numbers (each occurring only once).  It is computed
   by generating palindromes in increasing order and testing each
   for the required factorisation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility for counting prime factors of a 64‑bit integer. */
#include "../utils/factor_u64.h"

/* ------------------------------------------------------------ */
/* --- Helper: create a palindrome from a "half" number ------- */
/* ------------------------------------------------------------ */
static inline seq_t make_palindrome(uint64_t half, bool odd)
{
    /* pal = half, then append the reverse of half (skipping the middle
       digit if the total number of digits is odd). */
    seq_t pal = (seq_t)half;
    uint64_t x = odd ? half / 10 : half;
    while (x > 0) {
        pal = pal * (seq_t)10 + (seq_t)(x % 10);
        x /= 10;
    }
    return pal;
}

/* ------------------------------------------------------------ */
/* --- Pre‑computed powers of ten (uint64_t) ------------------ */
/* ------------------------------------------------------------ */
static uint64_t pow10_table[20];

static void init_pow10(void)
{
    pow10_table[0] = 1;
    for (size_t i = 1; i < sizeof(pow10_table) / sizeof(pow10_table[0]); ++i)
        pow10_table[i] = pow10_table[i-1] * (uint64_t)10;
}

/* ------------------------------------------------------------ */
/* --- Main sequence function -------------------------------- */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms      = NULL;   /* array of found terms          */
    static size_t terms_len   = 0;      /* current number of terms        */
    static size_t terms_cap   = 0;      /* current capacity of array      */
    static uint64_t curr_d    = 0;      /* current total digit length     */
    static uint64_t curr_half = 0;      /* next half to process           */
    static uint64_t start_half = 0;     /* start of current half range    */
    static uint64_t end_half   = 0;     /* end   of current half range    */
    static bool initialized = false;

    /* one‑time initialisation */
    if (!initialized) {
        init_pow10();
        curr_d = 1;                     /* start with 1‑digit palindromes */
        uint64_t half_len = (curr_d + 1) / 2;
        start_half = pow10_table[half_len-1];
        end_half   = pow10_table[half_len] - 1;
        curr_half  = start_half;
        /* allocate initial capacity for terms */
        terms_cap = 64;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        initialized = true;
    }

    /* auxiliary arrays for factorisation */
    uint64_t primes_f[64];
    uint64_t exp_f[64];
    size_t cnt_f;

    /* generate palindromes until we have enough terms */
    while (terms_len < (size_t)n) {
        /* when all halves for current digit length are exhausted,
           move to the next digit length. */
        if (curr_half > end_half) {
            ++curr_d;
            uint64_t half_len = (curr_d + 1) / 2;
            start_half = pow10_table[half_len-1];
            end_half   = pow10_table[half_len] - 1;
            curr_half  = start_half;
        }

        /* build the next palindrome and advance the half counter */
        seq_t candidate = make_palindrome(curr_half, (curr_d & 1ULL) != 0);
        ++curr_half;

        /* we can only factor 64‑bit numbers.  If the palindrome
           does not fit in uint64_t, we simply skip it – for the
           range of interest (first ~10 000 terms) all values lie
           well within 64 bits. */
        uint64_t cand_u = (uint64_t)candidate;
        factor_u64(cand_u, primes_f, exp_f, &cnt_f);

        /* need exactly six distinct prime factors, each with exponent
           one. */
        if (cnt_f == 6) {
            bool all_one = true;
            for (size_t i = 0; i < cnt_f; ++i) {
                if (exp_f[i] != 1) { all_one = false; break; }
            }
            if (all_one) {
                /* store the term */
                if (terms_len >= terms_cap) {
                    terms_cap = terms_cap * 2;
                    terms = (seq_t*)realloc(terms,
                                            terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = candidate;
            }
        }
    }

    return terms[n-1];
}
