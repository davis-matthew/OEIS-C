/*
 * A181181  Product of the first n zero-free positive numbers with digital sum
 *          n.
 * https://oeis.org/A181181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of the first n zero‑free positive integers whose
 *            digit sum equals n. Offset: 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   minimal_number
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/minimal_number.h"

/* Convert the first `len` digits of `digits` into a positive integer. */
static seq_t
digits_to_value(const uint8_t *digits, size_t len)
{
    seq_t v = (seq_t)0;
    for (size_t i = 0; i < len; ++i)
        v = v * (seq_t)10 + (seq_t)digits[i];
    return v;
}

/* Product of the first n zero‑free numbers with digit sum n. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* out of range placeholder */

    idx_t need = n;
    seq_t product = (seq_t)1;

    idx_t min_len = (n + 8) / 9;          /* ceil(n/9) */
    size_t max_len = (size_t)n + 5;
    uint8_t *digits = (uint8_t *)malloc(max_len);
    if (!digits) return (seq_t)1;          /* on OOM return a placeholder */

    for (size_t len = (size_t)min_len; need > 0; ++len)
    {
        if (len > max_len)                 /* grow buffer if necessary */
        {
            max_len = len + 5;
            uint8_t *tmp = (uint8_t *)realloc(digits, max_len);
            if (!tmp) break;
            digits = tmp;
        }

        minimal_number(digits, len, n);

        while (need > 0)
        {
            seq_t val = digits_to_value(digits, len);
            product = product * val;
            if (--need == 0) break;

            bool found = false;
            for (long long pos = (long long)len - 1; pos >= 0; --pos)
            {
                uint8_t cur = digits[pos];
                if (cur == 9) continue;

                /* sum of digits before position pos */
                idx_t prefixSum = 0;
                for (idx_t j = 0; j < (idx_t)pos; ++j)
                    prefixSum += digits[j];

                for (uint8_t inc = 1; cur + inc <= 9; ++inc)
                {
                    uint8_t newDigit = cur + inc;
                    idx_t needed = n - (prefixSum + (idx_t)newDigit);
                    size_t right = len - (size_t)pos - 1;
                    if (needed < (idx_t)right || needed > 9ULL * right) continue;

                    /* construct minimal suffix (rightmost digits get increments first) */
                    idx_t leftover = needed - (idx_t)right;
                    for (size_t j = len; j > pos; --j)   /* j goes from len-1 down to pos+1 */
                    {
                        uint8_t add = leftover > 8 ? 8 : (uint8_t)leftover;
                        digits[j - 1] = 1 + add;
                        leftover -= (size_t)add;
                    }
                    digits[pos] = newDigit;
                    found = true;
                    break;
                }
                if (found) break;
            }
            if (!found) break;               /* no further numbers of this length */
        }
        if (need == 0) break;
    }

    free(digits);
    return product;
}
