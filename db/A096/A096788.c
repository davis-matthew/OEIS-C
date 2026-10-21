/*
 * A096788  Numbers m such that both m and m+1 are composite numbers whose sum
 *          is a prime of the form 4k+3.
 * https://oeis.org/A096788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A096788:
 *  Numbers m such that m and m+1 are composite and m+m+1 = 2m+1 is a prime of the form 4k+3.
 *  Equivalently: odd composite m with 2*m+1 prime.  The sequence is
 *  a(1)=9, a(2)=15, a(3)=21, … (offset 1).
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"          /* provides bool isPrime(idx_t) */

/* external typedefs supplied by the build system */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* Nothing is defined for n < 1;  callers will not request such values. */
    if (n == 0) return 0;

    /* ---------- static cache of already found terms ---------- */
    static seq_t *cache      = NULL; /* array of terms */
    static size_t cache_len  = 0;    /* number of terms stored   */
    static size_t cache_cap  = 0;    /* allocated capacity      */

    /* ---------- next odd number to test (smallest odd composite) ---------- */
    static idx_t next_candidate = 9;

    /* first allocation of the cache */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* ------------------- generate terms up to index n ------------------- */
    while (cache_len < (size_t)n) {

        /* loop until a valid term is found */
        while (true) {

            /* Ensure we keep only odd numbers */
            if ((next_candidate & 1ULL) == 0)
                next_candidate++;

            /* Skip primes; m must be composite (m>1) */
            if (next_candidate > 1 && !isPrime(next_candidate)) {
                idx_t sum = 2 * next_candidate + 1; /* 2m+1 */
                if (isPrime(sum)) {                 /* m and m+1 composite, 2m+1 prime */

                    /* store the term */
                    if (cache_len == cache_cap) {
                        cache_cap <<= 1;
                        cache = (seq_t *)realloc(cache,
                                                  cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = (seq_t)next_candidate;

                    /* advance to the next odd candidate for future searches */
                    next_candidate += 2;
                    break;      /* next outer while() iteration */
                }
            }
            /* either not composite or sum not prime – try next odd */
            next_candidate += 2;
        }
    }

    return cache[n - 1];
}
