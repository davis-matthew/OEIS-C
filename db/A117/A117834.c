/*
 * A117834  Triangular numbers with consecutive digits.
 * https://oeis.org/A117834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, more, nonn
 * Formula:   a(n) = T(n-1) for those n where the triangular number has
 *            consecutive decimal digits (ascending or descending, with wrap
 *            9→0)
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   get_digits, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    /* Current triangular index for next search */
    static idx_t m_current = 0;

    /* Convert to zero‑based index */
    idx_t target = n - 1;
    if (target < 0) return 0; /* undefined for n<1, return 0 */

    /* Grow cache if needed */
    if (cache_len <= (size_t)target) {
        /* Make sure cache has enough capacity */
        if (cache_len >= cache_cap) {
            /* grow_seq_array doubles capacity */
            grow_seq_array(&cache, &cache_cap, cache_len + 1);
        }

        /* Temporary buffer for digit extraction */
        uint8_t buf[1024];
        size_t len;

        /* Search until we have enough terms */
        while (cache_len <= (size_t)target) {
            /* Compute current triangular number T(m_current) */
            seq_t t = ((seq_t)m_current) * (((seq_t)m_current) + 1) / 2;

            /* Extract digits */
            get_digits(t, buf, &len);

            /* Check for consecutive digits */
            bool consec = true;
            if (len > 1) {
                int diff0 = (buf[1] - buf[0] + 10) % 10;
                if (diff0 != 1 && diff0 != 9) consec = false;
                else {
                    for (size_t i = 1; i + 1 < len && consec; ++i) {
                        int d = (buf[i + 1] - buf[i] + 10) % 10;
                        if (d != diff0) consec = false;
                    }
                }
            }

            /* Store if consecutive */
            if (consec) {
                cache[cache_len++] = t;
                if (cache_len >= cache_cap) {
                    grow_seq_array(&cache, &cache_cap, cache_len + 1);
                }
            }

            /* Next triangular index */
            ++m_current;
        }
    }

    return cache[target];
}
