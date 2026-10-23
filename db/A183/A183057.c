/*
 * A183057  Cyclops emirps.
 * https://oeis.org/A183057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 4, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   digit_counts, digit_len, get_digits, is_prime_big, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n‑th element of OEIS sequence A183057
    (intersection of emirps A006567 and cyclops numbers A134808).
    The sequence is built on demand: successive integers are tested
    for primality, cyclops property (odd length, exactly one zero in the
    middle), and emirp property (reverse is a different prime).  */
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/rev_decimal.h"
#include "../utils/digit_counts.h"
#include "../utils/digit_len.h"
#include "../utils/get_digits.h"

/* Static cache for memoisation. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_candidate = 2;   /* next integer to test */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        /* The build guarantees that realloc succeeds.  */
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th (1‑based) term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for zero, but return 0 safely */
        return 0;

    /* Grow the cache until we have at least n terms. */
    if (cache_len < (size_t)n) {
        ensure_cache((size_t)n);
        while (cache_len < (size_t)n) {
            ++next_candidate;

            /* Skip even numbers > 2. */
            if (next_candidate > 2 && (next_candidate & 1) == 0)
                continue;

            /* Candidate must be prime. */
            if (!is_prime_big((seq_t)next_candidate))
                continue;

            /* Cyclops property: odd length, exactly one zero, and the zero
               is the middle digit. */
            int len = digit_len((seq_t)next_candidate);
            if ((len & 1) == 0)            /* even length → no exact middle */
                continue;

            unsigned char cnt[10];
            digit_counts((seq_t)next_candidate, cnt);
            if (cnt[0] != 1)               /* exactly one zero required */
                continue;

            /* Ensure the zero is at the centre. */
            uint8_t digits[128];
            size_t dlen = 0;
            get_digits((seq_t)next_candidate, digits, &dlen);
            if (digits[len / 2] != 0)
                continue;

            /* Emirp test: reversal must be a different prime. */
            seq_t rev = rev_decimal((seq_t)next_candidate);
            if (rev == (seq_t)next_candidate)    /* palindrome → not emirp */
                continue;
            if (!is_prime_big(rev))
                continue;

            /* All conditions satisfied – store the term. */
            cache[cache_len++] = (seq_t)next_candidate;
        }
    }

    return cache[n - 1];
}
