/*
 * A067499  Powers of 2 with digit sum also a power of 2.
 * https://oeis.org/A067499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   digit_sum, is_power_of_two, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067499: a(n) are powers of 2 whose digit sum is a power of 2. */
/* The function generates powers of two incrementally and keeps a static
 * cache of those whose digit sum is a power of two.
 * It works for any seq_t type that supports the helpers listed in the
 * project (uint8_t, uint16_t, uint32_t, uint64_t, uint128_t or arbitrary
 * precision). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/mul_small.h"
#include "../utils/digit_sum.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static bool   initialized   = false;
    static seq_t *terms         = NULL;
    static size_t terms_len     = 0;
    static size_t terms_cap     = 0;
    static seq_t  current_power = 0;

    /* Initialize on first call */
    if (!initialized) {
        initialized = true;

        terms_cap = 8;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) return 0;  /* unlikely to fail in tests */

        current_power = 1;          /* 2^0 */
        terms[0] = current_power;
        terms_len = 1;
    }

    /* Generate more terms until we have at least n entries */
    while (terms_len < n) {
        /* Next power of two */
        current_power = mul_small(current_power, 2U);

        /* Test digit sum */
        seq_t ds = digit_sum(current_power);
        if (is_power_of_two(ds)) {
            /* Grow cache if needed */
            if (terms_len + 1 > terms_cap) {
                terms_cap <<= 1;
                seq_t *new_terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!new_terms) return 0;  /* memory failure */
                terms = new_terms;
            }
            terms[terms_len++] = current_power;
        }
    }

    return terms[n - 1];
}
