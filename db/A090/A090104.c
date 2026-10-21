/*
 * A090104  Decimal value of maximal digit of the n-th primorial when written
 *          in base n.
 * https://oeis.org/A090104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = maximal digit in base n representation of the product of
 *            the first n primes (the n‑th primorial)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   get_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_first_n_primes.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;           /* by definition a(1)=1 */

    /* Get the first n primes. */
    idx_t *primes = get_first_n_primes(n);
    if (!primes) return (seq_t)0;

    /* Represent the current product in base n as an array of digits.
       digits[0] is the least‑significant digit. */
    size_t cap = 8;
    uint32_t *digits = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!digits) {
        free(primes);
        return (seq_t)0;
    }
    digits[0] = 1;
    size_t len = 1;

    for (size_t pi = 0; pi < (size_t)n; ++pi) {
        uint64_t p = (uint64_t)primes[pi];
        uint64_t carry = 0;
        for (size_t d = 0; d < len; ++d) {
            uint64_t prod = (uint64_t)digits[d] * p + carry;
            digits[d] = (uint32_t)(prod % (uint64_t)n);
            carry    = prod / (uint64_t)n;
        }
        while (carry > 0) {
            if (len == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(digits, cap * sizeof(uint32_t));
                if (!tmp) {
                    free(digits);
                    free(primes);
                    return (seq_t)0;
                }
                digits = tmp;
            }
            digits[len++] = (uint32_t)(carry % (uint64_t)n);
            carry /= (uint64_t)n;
        }
    }

    /* Find the maximal digit. */
    uint32_t maxdigit = 0;
    for (size_t d = 0; d < len; ++d)
        if (digits[d] > maxdigit) maxdigit = digits[d];

    free(digits);
    free(primes);
    return (seq_t)maxdigit;
}
