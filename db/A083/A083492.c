/*
 * A083492  Multiples of 4 in which there is no common digit in successive
 *          terms.
 * https://oeis.org/A083492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth positive multiple of 4 such that the set of
 *            decimal digits of a(n) does not intersect the set of digits of
 *            a(n‑1). a(1) = 4. The implementation keeps a dynamic array of
 *            previously generated terms and re‑uses it for subsequent calls
 * Range:     u8 n <= 12, u16 n <= 32, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static unsigned digit_mask(seq_t x)
{
    unsigned m = 0;
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % 10);
        m |= 1U << d;
        x /= 10;
    }
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */

    static seq_t *cache      = NULL;      /* stored terms */
    static size_t   len      = 0;         /* number of terms stored */
    static size_t   cap      = 0;         /* allocated capacity */
    static unsigned last_mask = 0;        /* mask of last stored term */
    static idx_t    mult_idx    = 1;      /* current multiple index (k in 4*k) */

    /* grow array if necessary (realloc cast for C compatibility) */
    if (len >= cap) {
        size_t new_cap = (cap == 0) ? 16 : cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        seq_t cand = (seq_t)4 * (seq_t)mult_idx;
        unsigned m = digit_mask(cand);

        if (len == 0 || (m & last_mask) == 0) {
            cache[len++] = cand;
            last_mask = m;
        }
        ++mult_idx;
    }

    return cache[n - 1];
}
