/*
 * A055604  Primes such that (a) reversal of digits gives a prime, (b)
 *          deleting any digit gives a prime and (c) reversing digits and
 *          deleting any digit gives a prime.
 * https://oeis.org/A055604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   digit_len, get_digits, is_prime_big, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This code computes OEIS sequence A055604: primes p
   such that (a) rev(p) is prime, (b) deleting any one digit of p
   yields a prime, and (c) deleting any one digit of rev(p) yields a
   prime. The function searches term by term in order, caching the
   results. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"
#include "../utils/rev_decimal.h"
#include "../utils/digit_len.h"
#include "../utils/get_digits.h"

/* Return true if p satisfies the three properties. */
static bool good_prime(seq_t p)
{
    /* (a) reversed digits must be prime */
    seq_t rev = rev_decimal(p);
    if (!is_prime_big(rev))
        return false;

    /* Buffer to hold the decimal digits of p */
    static size_t buf_cap = 0;
    static uint8_t *digits   = NULL;
    static uint8_t *rev_digits = NULL;

    int len_int = digit_len(p);
    size_t len = (size_t)len_int;
    if (len > buf_cap) {
        buf_cap = len;
        digits   = (uint8_t *)realloc(digits,   buf_cap);
        rev_digits = (uint8_t *)realloc(rev_digits, buf_cap);
    }

    /* Load digits of p */
    get_digits(p, digits, &len);

    /* Build the array of digits of rev(p) */
    for (size_t i = 0; i < len; ++i)
        rev_digits[i] = digits[len - 1 - i];

    /* (b) delete each digit of p and test */
    for (size_t del = 0; del < len; ++del) {
        seq_t q = 0;
        for (size_t j = 0; j < len; ++j) {
            if (j == del) continue;
            q = q * 10 + digits[j];
        }
        if (!is_prime_big(q))
            return false;
    }

    /* (c) delete each digit of rev(p) and test */
    for (size_t del = 0; del < len; ++del) {
        seq_t q = 0;
        for (size_t j = 0; j < len; ++j) {
            if (j == del) continue;
            q = q * 10 + rev_digits[j];
        }
        if (!is_prime_big(q))
            return false;
    }

    return true;
}

/* Return the nth term (0‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Cached terms */
    static seq_t *cache = NULL;
    static size_t cache_cnt = 0;
    static size_t cache_cap = 0;

    /* Last checked integer (uint64_t) – we only need to test
       positive integers and skip even ones beyond 2. */
    static uint64_t last_checked = 1;   /* start after 1 */

    /* Initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Grow cache until it holds term n */
    while (cache_cnt <= (size_t)n) {
        /* Find next prime candidate */
        do {
            ++last_checked;
            if (last_checked > 2 && (last_checked & 1) == 0)
                continue;   /* skip even numbers */
        } while (!is_prime_big((seq_t)last_checked));

        /* Check additional conditions */
        if (good_prime((seq_t)last_checked)) {
            if (cache_cnt >= cache_cap) {
                cache_cap <<= 1;   /* double capacity */
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_cnt++] = (seq_t)last_checked;
        }
    }

    return cache[(size_t)n];
}
