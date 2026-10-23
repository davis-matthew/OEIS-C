/*
 * A037978  Numbers whose maximal base 4 run length is 3.
 * https://oeis.org/A037978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   This OEIS sequence lists the integers whose base‑4
 *            representation contains at least one run of exactly three equal
 *            consecutive digits and no run longer than three
 * Range:     u8 n <= 21, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Check whether the base‑4 representation of v satisfies the run‑length
   property described above.  The function returns true iff there is at
   least one run of length exactly three and every run has length ≤ 3. */
static bool has_max_run3(seq_t v)
{
    if (v == 0)
        return false;

    int prev_digit = -1;
    unsigned run_len = 0;
    bool found3 = false;

    while (v > 0) {
        int d = (int)(v % 4);
        if (d == prev_digit) {
            ++run_len;
        } else {
            if (run_len > 3)
                return false;
            if (run_len == 3)
                found3 = true;
            prev_digit = d;
            run_len = 1;
        }
        v = v / 4;
    }

    /* process the final run */
    if (run_len > 3)
        return false;
    if (run_len == 3)
        found3 = true;

    return found3;
}

/* Static cache of already computed terms.  The capacity is bounded to
   a large enough number to cover the required queries (n up to ~10 000). */
#define CACHE_CAP 20000
static seq_t cache[CACHE_CAP];
static idx_t cache_count = 0;

/* Return the n‑th term of the sequence (a(1) = 21). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n = 0, harmless for callers */

    /* If we already have the needed term, return it immediately. */
    if (cache_count >= n)
        return cache[n - 1];

    /* Resume searching from the next integer after the last stored value. */
    seq_t candidate = cache_count ? cache[cache_count - 1] + 1 : 1;

    while (cache_count < n) {
        if (has_max_run3(candidate)) {
            if (cache_count < CACHE_CAP)
                cache[cache_count] = candidate;
            ++cache_count;
        }
        ++candidate;
    }

    return cache[n - 1];
}
