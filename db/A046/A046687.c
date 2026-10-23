/*
 * A046687  Numbers k such that k and sum of 4th powers of divisors of k are
 *          relatively prime.
 * https://oeis.org/A046687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 153, u64 n <= 9980,
 *            u128 n <= 9980, big any n
 * Helpers:   gcd, sigma_fourth_powers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k and the sum of fourth powers of its divisors are coprime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/sigma_fourth_powers.h"

seq_t a(idx_t n)
{
    static seq_t *values   = NULL;
    static size_t capacity = 0, size = 0;
    static uint64_t current = 0;  /* last integer examined */

    while (size < n)
    {
        ++current;
        seq_t k = (seq_t)current;
        seq_t s = sigma_fourth_powers(current);

        if (gcd(k, s) == (seq_t)1)
        {
            if (size + 1 > capacity)
            {
                size_t new_cap = capacity == 0 ? 16 : capacity * 2;
                values = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
                capacity = new_cap;
            }
            values[size++] = k;
        }
    }

    return values[n - 1];
}
