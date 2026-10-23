/*
 * A133740  Primes which are the sum of four positive 4th powers.
 * https://oeis.org/A133740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133740: primes which are the sum of four positive fourth powers
   – computed by a sieve and dynamic programming for sums of exactly
   four biquadratic numbers.  The values are generated lazily and cached
   in a static array.  When the cached size is insufficient the bound
   is doubled and the array is recomputed.  This implementation works
   for C11 and C++17, accepts arbitrary precision seq_t and uses
   the helper types supplied by the build system.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* dynamic programming: shift one bitset by a given amount */
static void shift_or(uint64_t *dest, const uint64_t *src,
                     size_t src_len, size_t dest_len, size_t shift)
{
    size_t wshift = shift >> 6;
    size_t bshift = shift & 63U;
    if (bshift == 0) {
        for (size_t i = 0; i + wshift < src_len && i + wshift < dest_len; ++i)
            dest[i + wshift] |= src[i];
    } else {
        for (size_t i = 0; i + wshift < src_len && i + wshift < dest_len; ++i) {
            uint64_t low = src[i] << bshift;
            dest[i + wshift] |= low;
            if (i + wshift + 1 < dest_len)
                dest[i + wshift + 1] |= src[i] >> (64 - bshift);
        }
    }
}

/* build the list of primes up to 'bound' that can be represented as
   a sum of four positive fourth powers; the result is stored in
   *seq_ptr and the number of elements in *cnt_ptr. */
static void build_sequence(seq_t **seq_ptr, size_t *cnt_ptr, size_t bound)
{
    /* pre‑compute fourth powers up to bound */
    uint64_t max_a = 0;
    while ((max_a + 1) * (max_a + 1) * (max_a + 1) * (max_a + 1) <= bound)
        ++max_a;
    uint64_t *pow4 = (uint64_t *)malloc((max_a + 1) * sizeof(uint64_t));
    for (uint64_t a = 1; a <= max_a; ++a)
        pow4[a] = a * a * a * a;

    /* DP using two bitsets: prev holds reachable sums with k terms,
       curr holds reachable sums with k+1 terms. */
    size_t bits = (bound >> 6) + 1;
    uint64_t *prev = (uint64_t *)calloc(bits, sizeof(uint64_t));
    uint64_t *curr = (uint64_t *)calloc(bits, sizeof(uint64_t));
    prev[0] = 1ULL;                     /* sum 0 with 0 terms */

    for (int term = 0; term < 4; ++term) {
        for (size_t i = 0; i < bits; ++i) curr[i] = 0;
        for (uint64_t a = 1; a <= max_a; ++a)
            shift_or(curr, prev, bits, bits, pow4[a]);
        uint64_t *tmp = prev; prev = curr; curr = tmp;
    }

    /* sieve primes up to bound */
    uint8_t *is_prime = (uint8_t *)calloc(bound + 1, sizeof(uint8_t));
    for (size_t i = 2; i <= bound; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= bound; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= bound; j += i)
                is_prime[j] = 0;
        }
    }

    /* collect primes that are reachable */
    size_t est = bound / 100 + 1;            /* rough estimate */
    size_t cap = est < 4000 ? 4000 : est;
    *seq_ptr = (seq_t *)malloc(cap * sizeof(seq_t));
    *cnt_ptr = 0;
    for (size_t p = 2; p <= bound; ++p) {
        if (!is_prime[p]) continue;
        if (prev[p >> 6] & (1ULL << (p & 63))) {
            if (*cnt_ptr == cap) {
                cap <<= 1;
                *seq_ptr = (seq_t *)realloc(*seq_ptr, cap * sizeof(seq_t));
            }
            (*seq_ptr)[*cnt_ptr] = (seq_t)p;
            ++(*cnt_ptr);
        }
    }

    /* free temporaries */
    free(pow4);
    free(prev);
    free(curr);
    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t bound = 2000000U;   /* initial bound */

    /* rebuild if we need more terms */
    while (seq_len < (size_t)n) {
        build_sequence(&seq, &seq_len, bound);
        if (seq_len < (size_t)n) {
            /* not enough: double the bound and try again */
            bound <<= 1;
            free(seq);
            seq = NULL;
            seq_len = 0;
        }
    }
    return seq[n - 1];
}
