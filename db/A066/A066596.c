/*
 * A066596  Primes which can be expressed as concatenation of powers of 5 and
 *          0's.
 * https://oeis.org/A066596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   free_seq_array, is_prime_big, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066596 – primes that can be written as a concatenation of powers of 5
 * and zeros.  This implementation generates prime numbers in increasing order,
 * tests whether each one satisfies the digit pattern, and caches the found
 * primes so repeated calls to a(idx_t n) are fast and order‑independent. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

/* Precomputed decimal strings for the first few powers of 5. */
static const char *const POW5_STRS[] = {
    "1", "5", "25", "125", "625",
    "3125", "15625", "78125", "390625", "1953125", "9765625"
};
static const size_t POW5_LEN[] = {
    1, 1, 2, 3, 3,
    4, 5, 5, 6, 7, 7
};
#define POW5_COUNT (sizeof(POW5_STRS)/sizeof(POW5_STRS[0]))

/* Check whether a 64‑bit unsigned integer can be written as a concatenation
 * of the above powers of 5 and/or single zeros. */
static bool matches_pattern(uint64_t n) {
    char digits[25];   /* sufficient for 64‑bit values (≤20 decimal digits) */
    size_t len = 0;
    if (n == 0) return false;
    while (n > 0) {
        digits[len++] = '0' + (n % 10);
        n /= 10;
    }
    for (size_t i = 0; i < len / 2; ++i) {      /* reverse to MSB → LSB order */
        char tmp = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = tmp;
    }

    bool dp[26] = { false };
    dp[0] = true;
    for (size_t pos = 0; pos < len; ++pos) {
        if (!dp[pos]) continue;
        if (digits[pos] == '0')
            dp[pos + 1] = true;                      /* single zero block */

        for (size_t b = 0; b < POW5_COUNT; ++b) {    /* power‑of‑5 blocks */
            size_t blen = POW5_LEN[b];
            if (pos + blen > len) continue;
            const char *p = POW5_STRS[b];
            size_t k;
            for (k = 0; k < blen; ++k)
                if (digits[pos + k] != p[k]) break;
            if (k == blen)
                dp[pos + blen] = true;
        }
    }
    return dp[len];
}

/* ---------- cache for found primes ------------------------------------- */
static seq_t *seq_cache = NULL;   /* array of found terms */
static size_t seq_capacity = 0;   /* allocated capacity */
static size_t seq_size = 0;       /* number of stored terms */
static uint64_t next_candidate = 2; /* next integer to test for primality */

/* Grow the cache to hold at least 'needed' items. */
static void ensure_cache(size_t needed) {
    if (seq_capacity >= needed) return;
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = seq_alloc_new_array(new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    for (size_t i = 0; i < seq_size; ++i)
        tmp[i] = seq_cache[i];
    if (seq_cache) free_seq_array(seq_cache);
    seq_cache = tmp;
    seq_capacity = new_cap;
}

/* Generate primes until we have at least 'n' of them. */
static void fill_up_to(idx_t n) {
    ensure_cache((size_t)n);
    while (seq_size < (size_t)n) {
        if (matches_pattern(next_candidate) &&
            is_prime_big((seq_t)next_candidate)) {
            seq_cache[seq_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* ---------- public interface ------------------------------------------ */
seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1. */
    fill_up_to(n);
    return seq_cache[n - 1];
}
