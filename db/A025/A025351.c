/*
 * A025351  Numbers that are the sum of 3 distinct nonzero squares in 5 or
 *          more ways.
 * https://oeis.org/A025351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A025351 – Numbers that are the sum of 3 distinct non‑zero squares
    in 5 or more unordered ways.
    This routine precomputes the list of such numbers up to the first
    TARGET_COUNT (=10000) terms.  It then returns the nth term
    (1‑based).  The implementation uses only integer arithmetic and
    works for any of the allowed integer types for seq_t and idx_t.  */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ------------------------------------------------------------ */
/* Configuration                                                     */
/* ------------------------------------------------------------ */
#define TARGET_COUNT   10000          /* number of terms that must be found   */
static const idx_t INITIAL_MAX   = 500000;  /* initial search bound (must be >= maximal term needed) */

/* ------------------------------------------------------------ */
/* Global data                                                       */
/* ------------------------------------------------------------ */
static seq_t  qualifiers[TARGET_COUNT];  /* storage for the sequence values            */
static size_t qualifiers_count   = 0;     /* actual number of gathered terms            */
static bool   initialized        = false;/* whether the precomputation has finished     */

/* ------------------------------------------------------------ */
/* Helper: integer square‑root floor                                    */
/* ------------------------------------------------------------ */
static idx_t isqrt_floor(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) r++;
    return r;
}

/* ------------------------------------------------------------ */
/* Precomputation: populate `qualifiers`                               */
/* ------------------------------------------------------------ */
static void precompute(void)
{
    idx_t search_max = INITIAL_MAX;

    while (true) {
        /* Allocate square values up to sqrt(search_max) */
        idx_t m = isqrt_floor(search_max);
        unsigned *sqs = (unsigned *)malloc(sizeof(unsigned) * (m + 1));
        if (!sqs) abort();
        sqs[0] = 0;
        for (idx_t i = 1; i <= m; ++i) sqs[i] = (unsigned)(i * i);

        /* Count representations: cnt[s] = number of unordered triples of
           distinct a<b<c with a^2+b^2+c^2 == s.                               */
        uint16_t *cnt = (uint16_t *)calloc((size_t)(search_max + 1),
                                          sizeof(uint16_t));
        if (!cnt) abort();

        for (idx_t a = 1; a <= m - 2; ++a) {
            unsigned sq_a = sqs[a];
            for (idx_t b = a + 1; b <= m - 1; ++b) {
                unsigned sum_ab = sq_a + sqs[b];
                if (sum_ab > search_max) break;
                for (idx_t c = b + 1; c <= m; ++c) {
                    unsigned sum = sum_ab + sqs[c];
                    if (sum > search_max) break;
                    /* Increment but never overflow the 16‑bit counter.      */
                    if (cnt[sum] < 0xFFFF) ++cnt[sum];
                }
            }
        }

        /* Collect the qualifying numbers in ascending order.               */
        qualifiers_count = 0;
        for (idx_t s = 1; s <= search_max; ++s) {
            if (cnt[s] >= 5) {
                if (qualifiers_count < TARGET_COUNT)
                    qualifiers[qualifiers_count++] = (seq_t)s;
                /* Stop storing if we've reached the required amount.        */
                if (qualifiers_count == TARGET_COUNT) break;
            }
        }

        free(cnt);
        free(sqs);

        if (qualifiers_count >= TARGET_COUNT) break;   /* finished */
        /* Otherwise enlarge the search range and try again.  */
        search_max *= 2;
    }

    initialized = true;
}

/* ------------------------------------------------------------ */
/* Public routine: returns a(n)  (1‑based)                        */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n < 1 */

    if (!initialized) precompute();

    if ((idx_t)qualifiers_count < n) return (seq_t)0; /* insufficient terms */
    return qualifiers[n - 1];
}
