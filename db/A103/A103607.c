/*
 * A103607  Write down the semiprimes but omit any semiprime (such as 46 or
 *          69) that is the concatenation of consecutive semiprimes.
 * https://oeis.org/A103607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A103607:  
   a(n) = the n-th semiprime that is NOT a concatenation of successive
   consecutive semiprimes.  The sequence is generated incrementally
   using static state so that calls in any order produce correct
   results. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits_idx.h"

/* true if x is the product of exactly two primes (possibly the same) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)
        return false;
    uint64_t d = 0;
    if (x % 2 == 0)
        d = 2;
    else {
        for (uint64_t dd = 3; dd <= x / dd; dd += 2) {
            if (x % dd == 0) {
                d = dd;
                break;
            }
        }
    }
    if (d == 0)
        return false;                      /* x is prime */
    uint64_t second = x / d;
    return is_prime_u64(second);
}

static seq_t pow10(idx_t exp) {
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i) res *= (seq_t)10;
    return res;
}

seq_t a(idx_t n) {
    /* Static state across calls. */
    static uint64_t semis[200000];         /* all generated semiprimes */
    static size_t   semis_cnt = 0;         /* number of stored semiprimes */

    static uint64_t incl[200000];          /* semiprimes that are kept */
    static size_t   incl_cnt = 0;          /* number of kept semiprimes */

    static uint64_t candidate = 4;         /* next integer to test */

    /* Pre‑computed powers of ten (as seq_t) for fast concatenation. */
    static seq_t   pow10arr[21];
    static bool    pow10_initted = false;
    if (!pow10_initted) {
        pow10arr[0] = 1;
        for (int i = 1; i <= 20; ++i)
            pow10arr[i] = pow10((idx_t)i);
        pow10_initted = true;
    }

    /* Generate terms until we have at least n kept semiprimes. */
    while (incl_cnt < n) {
        /* Find the next semiprime. */
        while (!is_semiprime_uint64(candidate))
            candidate++;

        uint64_t sp = candidate;           /* current semiprime */
        bool omit = false;
        int len_sp = num_digits_idx((idx_t)sp);

        /* Check whether sp is a concatenation of some consecutive
           subsequence of earlier semiprimes. */
        for (size_t i = 0; i < semis_cnt && !omit; ++i) {
            unsigned __int128 concat = 0;
            int len_concat = 0;
            for (size_t j = i; j < semis_cnt; ++j) {
                uint64_t sj = semis[j];
                int dj = num_digits_idx((idx_t)sj);
                if (len_concat + dj > len_sp)
                    break;

                unsigned __int128 pow10dj =
                    (unsigned __int128)pow10arr[dj];
                concat = concat * pow10dj + sj;
                len_concat += dj;

                if ((uint64_t)concat == sp) {
                    omit = true;
                    break;
                }
            }
        }

        /* If it is not omitted, store it in the output list. */
        if (!omit) {
            incl[incl_cnt++] = sp;
        }

        /* Always add the semiprime to the list of all seen semiprimes
           so it can be used for future concatenation checks. */
        semis[semis_cnt++] = sp;

        /* Advance to the next candidate. */
        ++candidate;
    }

    return (seq_t)incl[n - 1];
}
