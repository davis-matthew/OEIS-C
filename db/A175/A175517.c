/*
 * A175517  Duplicate of A163678.
 * https://oeis.org/A175517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 4, i32 n <= 12, i64 n <= 34, i128 n <= 34,
 *            big any n
 * Helpers:   digit_counts, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A175517
 *  a(n) = the nth smaller prime p such that the next consecutive prime q
 *  satisfies q - p = 18 and the decimal representations of p and q are
 *  permutations (anagrams).
 *
 *  The function computes the sequence lazily and caches the results,
 *  so repeated calls with arbitrary n are handled efficiently.
 */

#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_counts.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /*  static cache of found terms  */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /*  static state for generating primes  */
    static seq_t last_prime = 0;

    /*  Ensure we have computed at least n terms  */
    if (cache_len < (size_t)n) {
        /*  Grow the cache if needed  */
        if (cache_cap < cache_len + 1) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
        }

        /*  Main generation loop  */
        while (cache_len < (size_t)n) {

            /*  find next prime after last_prime  */
            seq_t cand;
            if (last_prime == 0) {
                cand = 2;
            } else {
                cand = last_prime + 1;
                if (cand % 2 == 0) cand++;                 /* start with odd candidate */
                while (!is_prime_big(cand)) cand += 2;     /* skip evens */
            }

            /*  check gap 18  */
            if (last_prime > 0 && (cand - last_prime) == (seq_t)18) {
                /*  check anagram: compare digit counts  */
                unsigned char cnt1[10] = {0};
                unsigned char cnt2[10] = {0};
                digit_counts(last_prime, cnt1);
                digit_counts(cand, cnt2);
                bool anagram = true;
                for (int i = 0; i < 10; ++i) {
                    if (cnt1[i] != cnt2[i]) { anagram = false; break; }
                }
                if (anagram) {
                    /*  Store the smaller prime  */
                    if (cache_len >= cache_cap) {
                        grow_seq(&cache, &cache_cap, cache_len + 1);
                    }
                    cache[cache_len++] = last_prime;
                }
            }

            /*  move to next prime  */
            last_prime = cand;
        }
    }

    return cache[(size_t)n - 1];
}
