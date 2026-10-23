/*
 * A118634  Start with 1 and repeatedly reverse the digits and add 38 to get
 *          the next term.
 * https://oeis.org/A118634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1, a(n) = reverse_digits(a(n‑1)) + 38 for n > 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   reverse_digits, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t cur_max = 0;

    /* Ensure capacity for index n (1‑based access) */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap ? (cap << 1) : 4;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        if (cap == 0)
            cache = seq_alloc(new_cap);
        else
            cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
    }

    /* Initialize first term if not yet done */
    if (cur_max == 0) {
        cache[1] = (seq_t)1;
        cur_max = 1;
    }

    /* Compute terms up to n */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev  = reverse_digits(prev);
        cache[i]   = rev + (seq_t)38;
    }
    cur_max = n;
    return cache[n];
}
