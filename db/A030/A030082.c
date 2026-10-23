/*
 * A030082  a(n) = prime^3 and digits of prime appear in a(n).
 * https://oeis.org/A030082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   get_digits, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = prime^3 such that each distinct digit of the prime appears in the cube.
   The terms are listed in increasing order of the qualifying primes. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/get_digits.h"

/* Test whether the prime p satisfies the digit‑in‑cube condition */
static bool prime_satisfies(uint64_t p)
{
    /* distinct digits of the prime */
    bool prime_digit[10] = {false};
    uint64_t tmp = p;
    if (tmp == 0) {
        prime_digit[0] = true;
    } else {
        while (tmp > 0) {
            prime_digit[tmp % 10] = true;
            tmp /= 10;
        }
    }

    /* cube of the prime */
    seq_t cube = (seq_t)p;
    cube *= (seq_t)p;          /* p^2  */
    cube *= (seq_t)p;          /* p^3  */

    /* digits present in the cube */
    uint8_t buf[64];
    size_t len;
    get_digits(cube, buf, &len);

    bool cube_digit[10] = {false};
    for (size_t i = 0; i < len; ++i)
        cube_digit[buf[i]] = true;

    /* every digit of the prime must appear in the cube */
    for (int d = 0; d < 10; ++d) {
        if (prime_digit[d] && !cube_digit[d])
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1 */
        return 0;

    /* static storage for computed terms */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;

    /* static storage for prime generation */
    static uint64_t next_candidate = 2;

    /* grow the terms array when needed */
    if (terms_len == terms_cap) {
        size_t newcap = terms_cap ? terms_cap * 2 : 32;
        terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
        terms_cap = newcap;
    }

    /* generate terms until we have at least n of them */
    while (terms_len < n) {
        /* find the next prime */
        uint64_t p = next_candidate;
        while (!is_prime(p))
            ++p;
        next_candidate = p + 1;

        /* test condition */
        if (prime_satisfies(p)) {
            seq_t cube = (seq_t)p;
            cube *= (seq_t)p;
            cube *= (seq_t)p;

            /* ensure capacity for the new term */
            if (terms_len == terms_cap) {
                size_t newcap = terms_cap ? terms_cap * 2 : 32;
                terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
                terms_cap = newcap;
            }

            terms[terms_len++] = cube;
        }
    }

    return terms[n - 1];
}
