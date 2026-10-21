/*
 * A050659  Numbers n such that number of primes produced according to rules
 *          stipulated in Honaker's A048853 is 8.
 * https://oeis.org/A050659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth term of A050659
 * (numbers producing exactly 8 distinct primes when each decimal
 * digit is replaced by all other digits, excluding leading zeros). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache   = NULL;      /* stored terms */
    static size_t cache_len = 0;       /* number of stored terms */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t   next      = 1;      /* next integer to test */

    /* Expand cache until we have at least n terms */
    while (cache_len < n)
    {
        /* current candidate number */
        seq_t cand = (seq_t)next;

        /* Extract decimal digits (little‑endian) */
        seq_t tmp = cand;
        unsigned char digits[32];
        size_t dlen = 0;
        if (tmp == 0)
        {
            digits[dlen++] = 0;
        }
        else
        {
            while (tmp > 0)
            {
                digits[dlen++] = (unsigned char)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Store distinct primes found for this candidate */
        seq_t primes[200];
        size_t pcount = 0;
        bool overflow = false;

        seq_t pow10 = 1;   /* 10^position for little‑endian pos */
        for (size_t pos = 0; pos < dlen && !overflow; ++pos)
        {
            unsigned char old_d = digits[pos];
            unsigned start = 0;
            /* Prevent leading‑zero replacement for the most significant
               digit (pos == dlen-1). */
            if (pos == dlen - 1) start = 1;

            for (unsigned new_d = start; new_d < 10; ++new_d)
            {
                if (new_d == old_d) continue;   /* skip original digit */

                /* New number after replacement */
                seq_t m = cand - (seq_t)old_d * pow10 + (seq_t)new_d * pow10;

                /* Skip the candidate itself and single‑digit results. */
                if (m == cand || m < 10) continue;

                if (!is_prime_big(m)) continue;

                /* Check for duplicates */
                bool seen = false;
                for (size_t k = 0; k < pcount; ++k)
                {
                    if (primes[k] == m) { seen = true; break; }
                }
                if (seen) continue;

                primes[pcount++] = m;
                if (pcount > 8)
                {
                    overflow = true;   /* too many primes; discard candidate */
                    break;
                }
            }
            pow10 *= 10;
        }

        /* Store candidate if it produced exactly 8 distinct primes */
        if (!overflow && pcount == 8)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp_ptr) exit(EXIT_FAILURE);
                cache = tmp_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }

        ++next;
    }

    return cache[n - 1];
}
