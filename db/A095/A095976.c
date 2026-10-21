/*
 * A095976  Numbers k such that (largest digit of k) + (largest digit of k+1)
 *          is prime.
 * https://oeis.org/A095976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A095976 (OEIS):  k such that largest digit(k) + largest digit(k+1) is prime.
    Computed by incremental search with a static cache.  */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: check if a small integer (0..18) is prime. */
static bool is_small_prime(int x) {
    /* primes ≤18: 2,3,5,7,11,13,17 */
    switch (x) {
        case 2: case 3: case 5: case 7: case 11: case 13: case 17:
            return true;
        default:
            return false;
    }
}

/* Compute largest decimal digit of a seq_t value. */
static unsigned largest_digit(seq_t v) {
    if (v == 0) return 0;
    unsigned maxd = 0;
    while (v > 0) {
        unsigned d = (unsigned)(v % 10);
        if (d > maxd) maxd = d;
        v /= 10;
    }
    return maxd;
}

/* Memoized incremental generation of A095976. */
seq_t a(idx_t n) {
    static seq_t *terms = NULL;        /* array of found terms */
    static size_t terms_len = 0;        /* number of terms stored */
    static size_t terms_cap = 0;        /* allocation size */
    static seq_t next_k = 1;           /* next candidate to test */

    if (n == 0) return 0;              /* offset is 1; value below 1 is undefined */

    /* Ensure capacity. */
    if (terms_len == 0 && terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return 0;          /* out of memory; unlikely in tests */
    }

    /* Grow if needed. */
    if (terms_len >= terms_cap) {
        terms_cap *= 2;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
        if (!terms) return 0;
    }

    /* Generate terms until we have at least n of them. */
    while (terms_len < n) {
        seq_t k = next_k++;
        unsigned d1 = largest_digit(k);
        unsigned d2 = largest_digit(k + 1);
        unsigned sum = d1 + d2;
        if (is_small_prime(sum)) {
            if (terms_len >= terms_cap) {
                terms_cap *= 2;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) return 0;
            }
            terms[terms_len++] = k;
        }
    }
    return terms[n - 1];
}
