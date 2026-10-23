/*
 * A036976  Becomes a prime if a 1 is inserted before each digit.
 * https://oeis.org/A036976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 12, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n such that inserting a '1' before each decimal digit of n produces a prime. */
/* This function generates terms lazily and memoises them across calls. */

#include <stdlib.h>
#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *terms = NULL;
    static idx_t    size   = 0;   /* number of cached terms */
    static idx_t    capacity = 0; /* allocated capacity */
    static idx_t    next_candidate = 1; /* integer to test next */

    /* Expand cache until it contains at least n+1 terms. */
    while (size <= n) {
        /* Build the number obtained by inserting '1' before each digit of next_candidate. */
        seq_t inserted = 0;
        idx_t temp = next_candidate;

        /* Extract digits into a small buffer (least significant first). */
        unsigned char digits[20];  /* 20 digits safely covers 64‑bit integers */
        idx_t len = 0;
        if (temp == 0) {
            digits[len++] = 0;
        } else {
            while (temp) {
                digits[len++] = temp % 10;
                temp /= 10;
            }
        }

        /* Process digits from most significant to least. */
        for (idx_t i = len; i-- > 0; ) {
            unsigned char d = digits[i];
            inserted = inserted * 100 + (10 + d);
        }

        /* Test primality of the inserted value. */
        if (is_prime_big(inserted)) {
            /* Grow cache if needed. */
            if (size == capacity) {
                capacity = capacity ? capacity * 2 : 4;
                terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
            }
            terms[size++] = (seq_t)next_candidate;
        }

        ++next_candidate;
    }

    return terms[n];
}
