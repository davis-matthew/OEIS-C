/*
 * A020591  Smallest nonempty set S containing prime divisors of 4k+6 for each
 *          k in S.
 * https://oeis.org/A020591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   _smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: iterative closure of prime divisors of 4k+6 starting from 2.
 * a(n) returns the n-th smallest prime that belongs to the set S
 * defined by the condition: for each k∈S, every prime divisor of 4k+6
 * also belongs to S.  The set S turns out to be finite and consists of
 * the nine primes 2,5,7,11,13,17,29,37,61.  This routine builds S once
 * (memoized across calls) and returns the requested entry. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"

static int cmp_seq(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    if (x < y) return -1;
    return x > y;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for indices < 1 */

    static seq_t *S = NULL;            /* growing array of primes */
    static size_t S_len = 0;
    static size_t S_cap = 0;
    static size_t proc = 0;            /* next prime to process */
    static bool built = false;
    static seq_t *sorted = NULL;

    if (!built) {
        /* initial allocation */
        S_cap = 16;
        S = (seq_t *)malloc(S_cap * sizeof(seq_t));
        S_len = 0;
        proc = 0;

        /* seed with smallest prime 2 */
        S[S_len++] = 2;

        /* iterative closure */
        while (proc < S_len) {
            seq_t p = S[proc++];
            uint64_t m = (uint64_t)p * 4ULL + 6ULL;  /* 4p+6 */
            uint64_t tmp = m;
            while (tmp > 1) {
                uint64_t spf = _smallest_prime_factor(tmp);
                /* add spf if not already present */
                bool found = false;
                for (size_t i = 0; i < S_len; ++i) {
                    if (S[i] == (seq_t)spf) { found = true; break; }
                }
                if (!found) {
                    if (S_len == S_cap) {
                        S_cap = S_cap ? S_cap * 2 : 16;
                        S = (seq_t *)realloc(S, S_cap * sizeof(seq_t));
                    }
                    S[S_len++] = (seq_t)spf;
                }
                while (tmp % spf == 0) tmp /= spf;
            }
        }

        /* sort the final set for easy indexing */
        qsort(S, S_len, sizeof(seq_t), cmp_seq);
        sorted = S;
        built = true;
    }

    if ((idx_t)(n - 1) < S_len)
        return sorted[n - 1];
    else
        return 0;   /* undefined for n beyond set size */
}
