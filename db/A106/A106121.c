/*
 * A106121  Primes with maximal digit > 4.
 * https://oeis.org/A106121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th prime whose decimal representation contains a digit > 4 */
/* (sequence A106121) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Dynamically grow the array of found primes */
static void add_prime(uint64_t **arr, size_t *cap, size_t *cnt, uint64_t val)
{
    if (*cnt >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        uint64_t *tmp = (uint64_t *)realloc(*arr, new_cap * sizeof(**arr));
        if (!tmp) return;           /* allocation failed – caller will free on exit */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*cnt)++] = val;
}

/* Return true if x contains at least one decimal digit > 4 */
static bool has_large_digit(uint64_t x)
{
    while (x) {
        uint64_t d = x % 10;
        if (d > 4) return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* The sequence has offset 1; values for n<1 are undefined */
    if (n == 0) return (seq_t)0;

    size_t primes_cap = 0, primes_cnt = 0;
    uint64_t *primes = NULL;

    uint64_t candidate = 2;
    size_t found = 0;               /* number of terms found so far */

    /* Main search loop */
    while (found < (size_t)n) {
        bool is_prime = true;

        /* 2 and 3 are prime but will not trigger the digit condition
           (they have max digit ≤ 4) */
        if (candidate == 2 || candidate == 3) {
            /* nothing special needed */
        } else if ((candidate & 1) == 0) {   /* even numbers are composite */
            is_prime = false;
        } else {
            /* trial division by previously found primes */
            for (size_t i = 0; i < primes_cnt; ++i) {
                uint64_t p = primes[i];
                if (p * p > candidate) break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        if (is_prime) {
            /* store prime for future trial divisions */
            add_prime(&primes, &primes_cap, &primes_cnt, candidate);

            if (candidate >= 5 && has_large_digit(candidate)) {
                ++found;
                if (found == (size_t)n) {
                    /* result found – free memory and return */
                    free(primes);
                    return (seq_t)candidate;
                }
            }
        }

        /* advance to next candidate: after 2 skip one, thereafter only odd numbers */
        candidate += (candidate == 2) ? 1 : 2;
    }

    /* Should never be reached */
    free(primes);
    return (seq_t)0;
}
