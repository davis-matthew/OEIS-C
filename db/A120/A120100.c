/*
 * A120100  Integers n for which the period of the decimal expansion of 1/n is
 *          6.
 * https://oeis.org/A120100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: integers n for which the period of the decimal expansion of 1/n is 6 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- helper to compute 10^exp mod mod ---------- */
static inline uint64_t mod_pow10(uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    for (uint64_t i = 0; i < exp; ++i)
        result = (unsigned __int128)result * 10 % mod;
    return result;
}

/* ----- static state for caching results ---------- */
static seq_t *seq_cache = NULL;      /* array of found terms         */
static size_t seq_len   = 0;          /* number of terms cached       */
static size_t seq_cap   = 0;          /* allocated capacity of cache  */
static idx_t  check_from = 1;        /* next integer to test         */

/* ----- core function ---------- */
seq_t a(idx_t n)
{
    /* Extend cache until we have at least n terms */
    while (seq_len < (size_t)n)
    {
        idx_t cand = check_from++;          /* candidate integer     */

        /* Reduce cand by removing all factors 2 and 5 */
        idx_t m = cand;
        while ((m & 1ull) == 0) m >>= 1;    /* divide by 2^a        */
        while (m % 5ull == 0) m /= 5ull;    /* divide by 5^b        */

        if (m == 1)                 /* terminating decimal → skip */
            continue;

        /* Test whether order of 10 modulo m is exactly 6 */
        if (mod_pow10(6, (uint64_t)m) != 1ULL)
            continue;              /* 10^6 ≠ 1 mod m */

        /* Ensure no smaller exponent gives 1 mod m */
        if ((uint64_t)10 % m == 1ULL)          continue;
        if ((unsigned __int128)10 * 10 % m == 1ULL)  continue;
        if ((unsigned __int128)10 * 10 * 10 % m == 1ULL)  continue;

        /* Candidate satisfies the condition – store it */
        if (seq_len == seq_cap) {
            seq_cap = seq_cap ? seq_cap * 2 : 16;
            seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        }
        seq_cache[seq_len++] = (seq_t)cand;
    }
    return seq_cache[n - 1];
}
