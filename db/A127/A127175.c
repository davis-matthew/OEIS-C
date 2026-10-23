/*
 * A127175  Primes whose "Look And Say" descriptions from left to right (in
 *          the sense of method B, i.e., digit-indication followed by
 *          frequency) are also primes.
 * https://oeis.org/A127175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   count_to_str, get_digits, is_prime_big, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth prime whose method‑B Look‑and‑Say
 * description (digit first, frequency second, left to right) is also
 * prime.  The function generates primes incrementally and keeps a
 * memoised list of the terms found. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/get_digits.h"
#include "../utils/seq_from_str.h"
#include "../utils/count_to_str.h"

/* Build the method‑B Look‑and‑Say description of a prime p.
 * The result is returned as a seq_t.  Digits are processed from most
 * significant to least significant. */
static seq_t build_description(seq_t p)
{
    /* Extract digits of p, least significant first (as used by get_digits). */
    uint8_t digits[128];
    size_t dlen = 0;
    get_digits(p, digits, &dlen);

    char buf[512];
    size_t pos = 0;

    /* Process digits from most significant to least. */
    for (size_t i = 0; i < dlen; ) {
        uint8_t d = digits[dlen - 1 - i];
        size_t j = i + 1;
        while (j < dlen && digits[dlen - 1 - j] == d)
            ++j;
        int count = (int)(j - i);

        buf[pos++] = '0' + d;                  /* digit character   */
        pos += count_to_str(count, buf + pos); /* frequency string  */
        i = j;
    }

    buf[pos] = '\0';
    return seq_from_str(buf);
}

/* Check whether a prime p satisfies the sequence property. */
static bool good_prime(seq_t p)
{
    seq_t desc = build_description(p);
    return is_prime_big(desc);
}

/* Memoised storage of terms found so far. */
static seq_t *terms      = NULL;
static size_t terms_cap  = 0;
static size_t terms_len  = 0;
static seq_t last_checked = 3;         /* next candidate to examine */

/* Grow the array of stored terms if needed. */
static void grow_terms(size_t new_len)
{
    if (new_len <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < new_len)
        new_cap *= 2;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    terms_cap = new_cap;
}

/* Return the nth term of the sequence (n >= 1).  The function
 * handles calls in any order by memoising previously computed terms. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    while (terms_len < (size_t)n) {
        seq_t cand = last_checked;

        /* Find next prime. */
        while (!is_prime_big(cand))
            ++cand;
        last_checked = cand + 1;

        if (good_prime(cand)) {
            grow_terms(terms_len + 1);
            terms[terms_len++] = cand;
        }
    }

    return terms[n - 1];
}
