/*
 * A182069  Triangle of numbers 2^i*C(n,i) mod 3 converted to decimal.
 * https://oeis.org/A182069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the base‑3 number whose i‑th digit is (2^i * C(n,i)) mod
 *            3 * (i runs from 0 to n, most significant digit is i = n). * The
 *            sequence satisfies a(3*n+2) = 13*a(3*n)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* 2^i mod 3 is 1 for even i, 2 for odd i.
 * mult2[x] = (2 * x) mod 3 for x in {0,1,2}. */
static const uint8_t mult2[3] = {0, 2, 1};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* computed a(k) for k = 0..m */
    static size_t cache_size = 0;    /* number of stored elements   */
    static uint8_t *row = NULL;      /* current binomial row mod 3 */
    static size_t row_len = 0;       /* length of row (current n+1) */

    /* extend cache and row up to n */
    if ((size_t)n >= cache_size)
    {
        size_t new_cache_size = (size_t)n + 1;
        seq_t *tmp_cache = (seq_t *)realloc(cache,
                                            new_cache_size * sizeof(seq_t));
        if (!tmp_cache && new_cache_size > 0) abort();
        cache = tmp_cache;
        cache_size = new_cache_size;

        for (size_t m = row_len; m <= (size_t)n; ++m)
        {
            /* grow row array to length m+1 */
            uint8_t *tmp_row = (uint8_t *)realloc(row,
                                                  (m + 1) * sizeof(uint8_t));
            if (!tmp_row && (m + 1) > 0) abort();
            row = tmp_row;

            /* set C(m,m) = 1 */
            row[m] = 1;

            /* update previous entries using Pascal's rule modulo 3 */
            for (size_t i = m; i > 0; --i)
            {
                if (i != m)               /* row[m] already set to 1 */
                    row[i] = (row[i] + row[i - 1]) % 3;
            }

            /* compute a(m) from the current row */
            seq_t val = 0;
            for (size_t i = 0; i <= m; ++i)
            {
                uint8_t digit = (i & 1) ? mult2[row[i]] : row[i];
                val = val * 3 + digit;
            }
            cache[m] = val;
        }
        row_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
