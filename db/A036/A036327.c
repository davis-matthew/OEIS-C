/*
 * A036327  Composite numbers n such that juxtaposition of prime factors of n
 *          has length 3.
 * https://oeis.org/A036327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   digitlen, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036327: Search for composite numbers whose prime‐factor
   concatenation has exactly 3 decimal digits. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/digitlen.h"

/* Cache of terms produced so far. */
static seq_t *seq      = NULL;
static size_t seq_len = 0;   /* number of terms stored (1‑based index) */
static size_t seq_cap = 0;   /* capacity of the array            */
static uint64_t next_candidate = 4;  /* first composite after 1 */

/* Make sure that seq contains at least 'need' terms (1‑based index). */
static void ensure_seq(idx_t need)
{
    while (seq_len < need) {
        /* Find next valid composite. */
        for (;;) {
            uint64_t n = next_candidate++;
            uint64_t pr[64];
            uint64_t ex[64];
            size_t  cnt = 0;

            factor_u64(n, pr, ex, &cnt);

            size_t total_factors = 0;
            int    sumlen       = 0;
            for (size_t i = 0; i < cnt; ++i) {
                total_factors += ex[i];
                sumlen += ex[i] * digitlen(pr[i]);
                if (sumlen > 3) break;
            }
            if (total_factors < 2) continue; /* skip primes and 1 */
            if (sumlen != 3)      continue; /* length condition fails */

            /* Accept the number. */
            if (seq_len == seq_cap) {
                size_t newcap = seq_cap ? seq_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                if (!tmp) abort();           /* allocation failure */
                seq    = tmp;
                seq_cap = newcap;
            }
            seq[seq_len++] = (seq_t)n;
            break;                          /* proceed to next term */
        }
    }
}

/* Public API: return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for index 0 */
    ensure_seq(n);
    return seq[n - 1];
}
