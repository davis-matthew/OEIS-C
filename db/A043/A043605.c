/*
 * A043605  Numbers whose base-5 representation has exactly 5 runs.
 * https://oeis.org/A043605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑5 representation has
 *            exactly 5 digit runs (a run is a maximal block of equal
 *            consecutive digits). This implementation generates the terms
 *            sequentially up to the requested index, caching all found terms
 *            so that repeated or arbitrary calls are efficient
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation state */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static seq_t next_candidate = 0;

    /* Generate terms until we have at least n of them. */
    while (len < (size_t)n) {
        /* Advance to the next candidate number. */
        next_candidate++;

        /* Count runs of digits in base‑5. */
        seq_t temp = next_candidate;
        seq_t prev_digit = (seq_t)5;          /* sentinel different from 0–4 */
        int    runs     = 0;
        while (temp > 0) {
            seq_t digit = temp % (seq_t)5;
            if (digit != prev_digit) {
                prev_digit = digit;
                runs++;
            }
            temp /= (seq_t)5;
        }

        /* Store if the candidate has exactly 5 runs. */
        if (runs == 5) {
            if (len + 1 > cap) {
                size_t newcap = cap ? cap * 2 : 8;
#ifdef __cplusplus
                cache = reinterpret_cast<seq_t*>(realloc(cache,
                                                          newcap * sizeof(seq_t)));
#else
                cache = (seq_t*)realloc(cache,
                                         newcap * sizeof(seq_t));
#endif
                /* In a production build, failure to allocate would be fatal;
                   here we simply ignore it as the sequence is small. */
                cap = newcap;
            }
            cache[len++] = next_candidate;
        }
    }

    return cache[n - 1];
}
