/*
 * A061372  Primes having only 0,4,6,8,9 as digits.
 * https://oeis.org/A061372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = the nth prime whose decimal digits are all in {0,4,6,8,9}
 *            * Computation: generate all integers of increasing decimal
 *            length that * contain only digits from {0,4,6,8,9} (with first
 *            digit from {4,6,8,9}), * test each for primality, and keep the
 *            primes in order. All primes found * are stored...
 * Range:     u8 n <= 1, u16 n <= 77, u32 n <= 41, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, push_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */
#include "../utils/push_prime.h"

/* Recursive generation of all numbers of a fixed length 'k'.
 * pos         – current digit position (0‑based)
 * cur         – number built so far
 * k           – desired length
 * array,len,cap – storage for primes
 */
static void gen_recursive(idx_t pos, seq_t cur, size_t k,
                          seq_t **array, size_t *len, size_t *cap)
{
    if (pos == k) {
        if (is_prime_big(cur))
            push_prime(array, len, cap, cur);
        return;
    }

    static const int first_digits[] = {4, 6, 8, 9};
    static const int other_digits[] = {0, 4, 6, 8, 9};
    const int *digits;
    int dcount;

    if (pos == 0) {
        digits = first_digits;
        dcount = 4;
    } else {
        digits = other_digits;
        dcount = 5;
    }

    for (int i = 0; i < dcount; ++i) {
        seq_t nxt = cur * 10 + digits[i];
        gen_recursive(pos + 1, nxt, k, array, len, cap);
    }
}

/* Generate all numbers of length 'k' (and all their primes).  Results
 * are appended to the static cache. */
static void generate_length(size_t k,
                            seq_t **array, size_t *len, size_t *cap)
{
    gen_recursive(0, 0, k, array, len, cap);
}

/* ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; outside range */

    /* static cache of found primes */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static size_t gen_len = 1;          /* last fully processed length */

    /* Ensure we have at least n primes */
    while (len < n) {
        size_t k = gen_len + 1;          /* next length to process */
        generate_length(k, &seq, &len, &cap);
        gen_len = k;
    }

    return seq[n - 1];
}
