/*
 * A057879  Primes with 3 distinct digits that remain prime (no leading zeros
 *          allowed) after deleting all occurrences of any one of its distinct
 *          digits.
 * https://oeis.org/A057879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime that has exactly three distinct decimal
 *            digits and remains prime after deleting all copies of each of
 *            its digits (no leading zeros). The function builds the sequence
 *            on demand
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

/* Static cache for the sequence values */
static seq_t *cache     = NULL;
static idx_t cache_size  = 0;
static idx_t cache_cap   = 0;
static idx_t current_candidate = 2;   /* next integer to test */

/* Grow the cache if needed.  Caller passes the number of extra slots
   required. */
static void ensure_capacity(idx_t needed)
{
    if (cache_size + needed > cache_cap) {
        idx_t new_cap = (cache_cap == 0 ? 1 : cache_cap << 1);
        while (new_cap < cache_size + needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* out of memory – terminate */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Ensure the cache contains the first n terms. */
    while (cache_size < n) {
        if (isPrime(current_candidate)) {
            /* Extract digits and find distinct digits (max 3). */
            int digits[60];            /* safe for 128‑bit numbers */
            int len = 0;
            bool seen[10] = {false};
            int uniq[3];
            int uniqCnt = 0;

            idx_t tmp = current_candidate;
            while (tmp > 0) {
                int d = (int)(tmp % 10);
                digits[len++] = d;
                if (!seen[d]) {
                    seen[d] = true;
                    uniq[uniqCnt++] = d;
                }
                tmp /= 10;
            }

            if (uniqCnt == 3) {
                bool ok = true;
                /* For each distinct digit, delete all its occurrences. */
                for (int k = 0; k < uniqCnt && ok; ++k) {
                    int del = uniq[k];
                    idx_t newp = 0;
                    bool first = true;
                    bool leadingZero = false;

                    for (int i = len - 1; i >= 0; --i) {
                        int d = digits[i];
                        if (d == del) continue;   /* skip this digit */

                        if (first) {
                            if (d == 0) {          /* leading zero not allowed */
                                leadingZero = true;
                                break;
                            }
                            first = false;
                            newp = d;
                        } else {
                            newp = newp * 10 + d;
                        }
                    }

                    if (leadingZero || newp < 2 || !isPrime(newp))
                        ok = false;
                }

                if (ok) {
                    ensure_capacity(1);
                    cache[cache_size++] = (seq_t)current_candidate;
                }
            }
        }

        /* Advance to next candidate: after 2 skip even numbers. */
        current_candidate += (current_candidate == 2 ? 1 : 2);
    }

    return cache[n - 1];
}
