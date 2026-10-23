/*
 * A024773  Every prefix and suffix prime in base 5 (written in base 5).
 * https://oeis.org/A024773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, to_base5_num
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A024773: primes that are left‑ and right‑truncatable in base 5.
 * The function returns the term represented in base 5 (i.e., the
 * decimal integer whose decimal digits are the base‑5 digits of the prime).
 *
 * For example: 67 decimal = 232 base 5 → a(5) = 232.
 *
 * No closed formula is known, so the function generates primes
 * and tests each one until the requested index is reached.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/to_base5_num.h"

/* --------------------------------------------------------------------- */
static bool is_truncatable(seq_t p)
/* Test whether the base‑5 representation of the decimal prime p is
 * both left‑ and right‑truncatable, i.e. every prefix and suffix (in
 * base 5) is itself a prime when interpreted as a decimal integer. */
{
    seq_t tmp = p;
    uint8_t digits[64];
    size_t len = 0;
    /* Extract base‑5 digits (least significant first). */
    while (tmp > (seq_t)0) {
        digits[len++] = (uint8_t)(tmp % (seq_t)5);
        tmp /= (seq_t)5;
    }
    if (len == 0)
        return false;   /* not expected for a prime > 0 */

    /* Reversed to most significant first. */
    for (size_t i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Prefixes. */
    seq_t val = 0;
    for (size_t i = 0; i < len; ++i) {
        val = val * (seq_t)5 + (seq_t)digits[i];
        if (!is_prime_big(val))
            return false;
    }

    /* Suffixes. */
    for (size_t start = 0; start < len; ++start) {
        val = 0;
        for (size_t j = start; j < len; ++j) {
            val = val * (seq_t)5 + (seq_t)digits[j];
            if (!is_prime_big(val))
                return false;
        }
    }
    return true;
}

/* --------------------------------------------------------------------- */
static seq_t *seq_cache = NULL;   /* computed terms */
static size_t seq_len   = 0;      /* number of computed terms */
static size_t seq_cap   = 0;      /* capacity of seq_cache */
static idx_t  next_prime = (idx_t)2;   /* next integer to test */

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;      /* undefined below offset */

    /* Generate terms until we have at least n. */
    while (seq_len < (size_t)n) {
        /* Find the next prime candidate. */
        seq_t cand = (seq_t)next_prime;
        while (!is_prime_big(cand)) {
            ++next_prime;
            cand = (seq_t)next_prime;
        }

        /* Test truncation property. */
        if (is_truncatable(cand)) {
            /* Grow cache if necessary. */
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 64;
                seq_t *new_arr = seq_alloc(new_cap);
                if (seq_cache != NULL) {
                    for (size_t i = 0; i < seq_len; ++i)
                        new_arr[i] = seq_cache[i];
                }
                seq_cache = new_arr;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = to_base5_num(cand);
        }
        ++next_prime;
    }
    return seq_cache[n - 1];
}
