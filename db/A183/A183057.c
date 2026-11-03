/*
 * A183057  Cyclops emirps.
 * https://oeis.org/A183057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   digit_counts, get_digits, is_prime_u64, mem, num_digits, reverse_digits
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

#include <stddef.h>
#include <stdint.h>
#include "../utils/digit_counts.h"
#include "../utils/get_digits.h"
#include "../utils/num_digits.h"
#include "../utils/reverse_digits.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
        void *tmp = mem_resize(seq_t, cache, new_cap);
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
            int len = num_digits((seq_t)next_candidate);
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
            seq_t rev = reverse_digits((seq_t)next_candidate);
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
