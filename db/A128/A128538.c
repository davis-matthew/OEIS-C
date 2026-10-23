/*
 * A128538  Number of prime factors (with multiplicity) of lucky numbers.
 * https://oeis.org/A128538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of prime factors (with multiplicity) of the n-th
 *            lucky number A000959(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_factor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_factor_count.h"

/* external linkage as requested */
seq_t a(idx_t n);

/* static storage for lucky numbers */
static uint64_t *lucky = NULL;    /* array of lucky numbers (0‑based) */
static size_t lucky_len = 0;      /* number of stored lucky numbers */

/* ensure that at least n lucky numbers are stored */
static void ensure_lucky(idx_t n)
{
    if (n < 1) return;
    if (lucky_len >= (size_t)n) return;   /* already available */

    /* initial bound – enough for n up to a few thousand */
    idx_t bound = (idx_t)(n * 15 + 100);
    if (bound < 1000) bound = 1000;

    while (1) {
        /* generate all odd numbers up to bound */
        uint64_t cnt_odds = bound / 2 + bound % 2;   /* number of odds <= bound */
        uint64_t *v = (uint64_t *)malloc(cnt_odds * sizeof(uint64_t));
        if (!v) return;   /* allocation failure – give up */

        size_t pos = 0;
        for (uint64_t i = 1; i <= bound; i += 2) v[pos++] = i;
        size_t len = pos;

        /* perform the lucky number sieve on the list of odds */
        size_t i = 1;          /* start from the second element (value 3) */
        while (i < len) {
            uint64_t step = v[i];
            if (step > len) break;   /* no more removal possible */

            size_t dst = 0;
            for (size_t j = 0; j < len; ++j) {
                if ((j + 1) % step != 0) v[dst++] = v[j];
            }
            len = dst;
            ++i;
        }

        if (len >= (size_t)n) {   /* we have enough lucky numbers */
            /* store them for future calls */
            size_t new_len = len + 1;                 /* allow index n-1 */
            uint64_t *tmp = lucky;
            lucky = (uint64_t *)realloc(lucky, new_len * sizeof(uint64_t));
            if (!lucky) {   /* allocation failure – restore */
                lucky = tmp;
                free(v);
                lucky_len = 0;
                return;
            }
            for (size_t k = 0; k < len; ++k) lucky[k] = v[k];
            lucky_len = len;
            free(v);
            break;
        }

        /* not enough – enlarge bound and try again */
        free(v);
        bound *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* defined only for n >= 1 */

    ensure_lucky(n);
    uint64_t lucky_no = lucky[n - 1];   /* nth lucky number (1‑based) */
    return prime_factor_count(lucky_no);
}
