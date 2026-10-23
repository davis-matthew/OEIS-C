/*
 * A044882  Numbers having, in base 11, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth number (in ascending order) whose base‑11
 *            representation has (sum of even run lengths) = (sum of odd run
 *            lengths)
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: check if the number val satisfies the run‑length condition */
static bool has_property(idx_t val)
{
    int even_sum = 0, odd_sum = 0;
    int run_len = 0;
    int last_digit = -1;          /* sentinel for no previous digit */

    while (val > 0) {
        int digit = (int)(val % 11);
        val = val / 11;

        if (digit == last_digit) {
            ++run_len;
        } else {
            if (run_len > 0) {
                if ((run_len & 1) == 0)
                    even_sum += run_len;
                else
                    odd_sum += run_len;
            }
            last_digit = digit;
            run_len = 1;
        }
    }

    /* process the final run */
    if (run_len > 0) {
        if ((run_len & 1) == 0)
            even_sum += run_len;
        else
            odd_sum += run_len;
    }

    return even_sum == odd_sum;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static idx_t next_candidate = 1;

    /* grow cache when needed */
    while (sz < n) {
        while (!has_property(next_candidate))
            ++next_candidate;

        /* grow underlying array */
        if (sz == cap) {
            size_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache)
                for (size_t i = 0; i < sz; ++i)
                    new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cap = new_cap;
        }

        /* store found term */
        cache[sz++] = (seq_t)next_candidate;
        ++next_candidate;
    }

    return cache[n - 1];
}
