/*
 * A103611  Smallest prime p with at least two non-overlapping occurrences of
 *          n in decimal representation of p.
 * https://oeis.org/A103611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime p that contains the decimal representation
 *            of n at least twice in p without overlap
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_len, get_prime, seq_to_dec_string
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_to_dec_string.h"
#include "../utils/get_prime.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined offset, safe return */

    /* Build the decimal pattern string of n. */
    char pat[32];
    seq_to_dec_string((seq_t)n, pat, sizeof(pat));
    const int pat_len = (int)strlen(pat);

    /* Iterate over primes in order (2, 3, 5, 7, …). */
    idx_t prime_idx = 1;                      /* get_prime(1) == 2 */
    for (;; ++prime_idx)
    {
        seq_t cp = get_prime(prime_idx);      /* candidate prime */

        /* Convert candidate to decimal string. */
        idx_t cp_len = digit_len(cp);
        char *buf = (char*)malloc(cp_len + 1);
        if (!buf) return 0;                  /* allocation failure -> abort */
        seq_to_dec_string(cp, buf, cp_len + 1);

        /* Search for two non‑overlapping occurrences of pat. */
        const char *p1 = strstr(buf, pat);
        if (p1)
        {
            const char *p2 = strstr(p1 + pat_len, pat);
            if (p2)
            {
                free(buf);
                return cp;                   /* found suitable prime */
            }
        }
        free(buf);
    }
}
