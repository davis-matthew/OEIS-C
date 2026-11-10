/*
 * A205083  Parity of A070885.
 * https://oeis.org/A205083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A070885(n) mod 2, where A070885(1) = 1 and A070885(n) =
 *            3*ceil(A070885(n-1)/2) for n > 1; computed with a multi-limb big
 *            integer
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 102 terms through n = 8597 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    size_t cap = (size_t)(n / 32) + 8;
    uint32_t *x = (uint32_t *)calloc(cap, sizeof(uint32_t));
    if (!x)
        abort();
    size_t len = 1;
    x[0] = 1;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* ceil: add 1 if odd */
        if (x[0] & 1u)
        {
            size_t j = 0;
            int carry = 1;
            while (carry && j < len)
            {
                x[j] += 1u;
                carry = (x[j] == 0u);
                ++j;
            }
            if (carry)
                x[len++] = 1u;
        }
        /* divide by 2 */
        for (size_t j = 0; j < len; ++j)
        {
            uint32_t hi = (j + 1 < len) ? (x[j + 1] << 31) : 0u;
            x[j] = (x[j] >> 1) | hi;
        }
        /* multiply by 3 */
        uint64_t c = 0;
        for (size_t j = 0; j < len; ++j)
        {
            uint64_t t = (uint64_t)x[j] * 3u + c;
            x[j] = (uint32_t)t;
            c = t >> 32;
        }
        if (c)
            x[len++] = (uint32_t)c;
    }

    seq_t r = (seq_t)(x[0] & 1u);
    free(x);
    return r;
}
