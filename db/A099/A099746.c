/*
 * A099746  Primes arising in A099744.
 * https://oeis.org/A099746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n‑th prime of the form 1 0 M 0 1, * where M is an
 *            odd‑length palindrome and the whole number is prime. * Generated
 *            by a simple incremental search
 * Range:     u8 none, u16 n <= 3, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   ensure_capacity, isPrimeGeneric, make_pal_from_half, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/isPrimeGeneric.h"
#include "../utils/ensure_capacity.h"
#include "../utils/seq_pow.h"
#include "../utils/make_pal_from_half.h"

/* small pow10 helper for idx_t (used to compute half limits) */
static inline idx_t pow10_u64(int e)
{
    idx_t r = 1;
    while (e-- > 0)
        r *= 10;
    return r;
}

/* Main sequence function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1‑indexed sequence, n == 0 is undefined */
    if (n == 0) return (seq_t)0;

    /* static cache for memoisation                                                 */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;

    /* state for the incremental generator                                         */
    static int    cur_l       = 0;      /* current odd length of M              */
    static int    half_len    = 0;      /* (cur_l+1)/2                          */
    static idx_t  cur_half    = 0;      /* next half to try                      */
    static idx_t  half_end    = 0;      /* last half for the current length      */
    static seq_t  base        = 0;      /* 10^(cur_l+3)                          */

    /* initialise generator on first call                                      */
    if (cur_l == 0) {
        cur_l = 1;                     /* start with length 1 (M of 1 digit)  */
        half_len = (cur_l + 1) / 2;    /* 1                                 */
        cur_half = pow10_u64(half_len - 1); /* 1 */
        half_end = pow10_u64(half_len) - 1; /* 9 */
        base = seq_pow((seq_t)10, (idx_t)(cur_l + 3));
    }

    /* generate primes until we have at least n terms                         */
    while (cache_len < n) {
        while (cur_half <= half_end && cache_len < n) {
            seq_t M = make_pal_from_half(cur_half, half_len);
            seq_t candidate = base + M * 100 + 1;   /* 1 0 M 0 1                */
            if (isPrimeGeneric(candidate)) {
                ensure_capacity(cache_len + 1, &cache_cap, (void **)&cache,
                                sizeof(seq_t));
                cache[cache_len++] = candidate;
            }
            ++cur_half;
        }

        /* move to next odd length if we finished the current one              */
        if (cur_half > half_end) {
            cur_l += 2;
            half_len = (cur_l + 1) / 2;
            cur_half = pow10_u64(half_len - 1);
            half_end = pow10_u64(half_len) - 1;
            base = seq_pow((seq_t)10, (idx_t)(cur_l + 3));
        }
    }

    return cache[n - 1];   /* 1‑based indexing                              */
}
