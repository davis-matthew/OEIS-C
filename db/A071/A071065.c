/*
 * A071065  A permutation of natural numbers: a(1)=2 a(n) is the smallest
 *          integer different from a(1), a(2), a(3), ..., a(n-1) such that
 *          a(n)+n is prime.
 * https://oeis.org/A071065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A071065:  a(1)=2.  For n>1, a(n) is the smallest positive integer not
   previously used such that a(n)+n is prime.
   The function is memoised and works on any call order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* simple deterministic prime test suitable for values up to ~2^64   */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)                 return false;
    if (n == 2)                return true;
    if ((n & 1ULL) == 0)       return false;   /* even numbers >2 are not prime */

    for (uint64_t d = 3; d <= n / d; d += 2) {
        if (n % d == 0)        return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static data shared between calls */
    static seq_t        *perm      = NULL;   /* permutation values, 0‑based   */
    static size_t        perm_cap  = 0;      /* allocated size of perm       */
    static size_t        perm_len  = 0;      /* number of terms computed    */

    static bool         *used      = NULL;   /* used numbers, index = value  */
    static size_t        used_cap  = 0;      /* allocated size of used       */

    /* ---------------------------------------------------------------- */
    /* ensure capacity of perm array for new terms                     */
    if (n > perm_cap) {
        size_t new_cap = perm_cap ? perm_cap : 1;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(perm, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        perm = tmp;
        perm_cap = new_cap;
    }

    /* ---------------------------------------------------------------- */
    /* initialise used array on first call                            */
    if (!used) {
        used_cap = 4;                      /* at least enough for small numbers */
        used = (bool*)calloc(used_cap, sizeof(bool));
        if (!used) exit(EXIT_FAILURE);
    }

    /* ---------------------------------------------------------------- */
    /* compute terms up to index n                                       */
    for (size_t idx = perm_len + 1; idx <= (size_t)n; ++idx) {

        seq_t chosen = 0;

        /* special case for a(1) = 2 */
        if (idx == 1) {
            chosen = (seq_t)2;

            /* make sure used array can hold value 2 */
            if (used_cap <= 2) {
                size_t new_used = used_cap;
                while (new_used <= 2) new_used <<= 1;
                bool *tmp = (bool*)realloc(used, new_used * sizeof(bool));
                if (!tmp) exit(EXIT_FAILURE);
                memset(tmp + used_cap, 0, (new_used - used_cap) * sizeof(bool));
                used = tmp;
                used_cap = new_used;
            }
            used[2] = true;
            perm[idx - 1] = chosen;
            continue;
        }

        /* search for the smallest unused integer satisfying the prime sum */
        uint64_t candidate = 1;
        for (;;) {
            /* grow used array if needed */
            if (candidate >= used_cap) {
                size_t new_used = used_cap;
                while (new_used <= (size_t)candidate) new_used <<= 1;
                bool *tmp = (bool*)realloc(used, new_used * sizeof(bool));
                if (!tmp) exit(EXIT_FAILURE);
                memset(tmp + used_cap, 0, (new_used - used_cap) * sizeof(bool));
                used = tmp;
                used_cap = new_used;
            }

            if (!used[candidate]) {
                uint64_t sum = candidate + (uint64_t)idx;
                if (is_prime_uint64(sum)) {
                    chosen = (seq_t)candidate;
                    used[candidate] = true;
                    break;
                }
            }
            ++candidate;
        }

        perm[idx - 1] = chosen;
    }

    perm_len = n;          /* all terms up to n are now stored */
    return perm[n - 1];
}
