/*
 * A075812  Palindromic odd numbers with exactly 2 prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A075812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 134, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   bigomega, rev_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th palindromic odd integer
   containing exactly two prime factors (counted with multiplicity).
   It generates palindromes in increasing order and keeps those
   that are odd and for which bigomega(n) == 2. */

/* Standard headers */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility to compute the function bigomega(n) (number of prime factors
   counted with multiplicity).  The appropriate header must be
   provided by the build system. */
#include "../utils/bigomega.h"
#include "../utils/rev_uint64.h"

/* Powers of ten up to 10^19 – enough for all palindromes that
   occur up to n = 10000. */
static const uint64_t pow10[20] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL,
    1000000ULL, 10000000ULL, 100000000ULL, 1000000000ULL,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL,
    1000000000000000000ULL, 10000000000000000000ULL
};

/* Reverse the decimal digits of a 64‑bit integer. */

/* Generator that yields all decimal palindromes in increasing order.
   Internally it works with 64‑bit integers and casts the result
   to seq_t when returning.  The first value generated is 1, the
   subsequent values are 2, 3, …, 9, 11, 22, … 99, 101, … etc. */
static seq_t next_palindrome(void)
{
    static uint64_t gen_len    = 1;   /* current palindrome length  */
    static uint64_t gen_prefix = 1;   /* leading half of the palindrome*/
    static int     gen_middle = 0;    /* middle digit for odd lengths */

    while (1) {
        /* ----- single‑digit palindromes ----- */
        if (gen_len == 1) {
            if (gen_prefix <= 9) {
                uint64_t val = gen_prefix++;
                return (seq_t)val;
            }
            /* Switch to length 2 after 9 */
            gen_len = 2;
            gen_prefix = pow10[0];   /* start with 1 */
            continue;
        }

        uint64_t half  = gen_len / 2;
        uint64_t start = pow10[half - 1];
        uint64_t end   = pow10[half] - 1;

        /* ----- odd length palindromes ----- */
        if (gen_len & 1) {
            if (gen_prefix == 0) gen_prefix = start;

            if (gen_prefix <= end) {
                if (gen_middle < 10) {
                    uint64_t val = gen_prefix * pow10[half + 1]
                                + (uint64_t)gen_middle * pow10[half]
                                + rev_uint64(gen_prefix);
                    gen_middle++;
                    return (seq_t)val;
                } else {
                    gen_middle = 0;
                    gen_prefix++;
                }
            } else {
                /* Move to next palindrome length */
                gen_len++;
                gen_prefix = 0;
                gen_middle = 0;
                continue;
            }
        }

        /* ----- even length palindromes ----- */
        else {
            if (gen_prefix == 0) gen_prefix = start;

            if (gen_prefix <= end) {
                uint64_t val = gen_prefix * pow10[half] + rev_uint64(gen_prefix);
                gen_prefix++;
                return (seq_t)val;
            } else {
                gen_len++;
                gen_prefix = 0;
                continue;
            }
        }
    }
}

/* Cache to store sequence values as they are discovered. */
static seq_t *cache      = NULL;  /* array of seq_t */
static size_t cachelen   = 0;     /* number of elements stored */
static size_t cachecap   = 0;     /* current capacity */

/* Ensure that the cache can hold at least one more element.  It uses
   the standard malloc/realloc helpers, compatible with any seq_t type
   (built‑in or arbitrary precision). */
static void ensure_cache_cap(void)
{
    if (cachecap == 0) {
        cachecap = 16;
        cache = (seq_t *)malloc(cachecap * sizeof(seq_t));
    } else if (cachelen == cachecap) {
        cachecap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, cachecap * sizeof(seq_t));
        cache = tmp;            /* realloc may return NULL – not handled
                                   because we never run out of memory
                                   for the required n. */
    }
}

/* Return the n‑th term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Offset starts at 1 – value unused */

    while (cachelen < (size_t)n) {
        seq_t pal = next_palindrome();      /* next palindrome as seq_t */

        /* For bigomega we need a 64‑bit value; the sequence terms
           up to n = 10000 are far below 2^64, so a simple cast suffices. */
        uint64_t val = (uint64_t)pal;

        /* Only odd numbers can qualify. */
        if ((val & 1ULL) && bigomega((idx_t)val) == (seq_t)2) {
            ensure_cache_cap();
            cache[cachelen++] = pal;
        }
    }
    return cache[n - 1];
}
