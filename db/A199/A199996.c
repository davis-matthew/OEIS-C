/*
 * A199996  Composite numbers whose multiplicative persistence is 6.
 * https://oeis.org/A199996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   digit_product, is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for composite numbers whose multiplicative persistence is 6
   (OEIS A199996).  The function builds the sequence lazily and
   memoises all computed terms.
   It returns the n‑th term for n >= 1.  Complexity is roughly linear
   in the number of candidates examined; sufficient for n up to
   ~10000 in <2 seconds.
*/
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"      /* is_prime(idx_t) */
#include "../utils/digit_product.h" /* digit_product(seq_t) */
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t *, size_t, size_t) */

/* ------------------------------------------------------------------ */
/* Memoisation structure                                             */
/* ------------------------------------------------------------------ */
static seq_t *seq  = NULL;          /* array of found terms            */
static size_t seq_count = 0;        /* number of terms stored          */
static size_t seq_capacity = 0;     /* allocated capacity of seq       */
static idx_t next_candidate = 2;    /* next integer to test           */

/* Ensure the sequence array has at least `needed` capacity */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_capacity) return;
    size_t new_cap = seq_capacity ? seq_capacity * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq = seq_realloc(seq, seq_capacity, new_cap);
    seq_capacity = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main OEIS function                                                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* safety for offset 1 (undefined) */

    /* Generate terms until we have at least n of them */
    while (seq_count < (size_t)n)
    {
        /* Find the next composite number with persistence 6 */
        while (1)
        {
            /* Skip 1 and test composites only */
            if (next_candidate > 1 && !is_prime(next_candidate))
            {
                seq_t x = (seq_t)next_candidate;
                int steps = 0;

                /* Apply the digit product until single digit or
                   6 steps have been performed. */
                while (x >= 10 && steps < 6)
                {
                    x = digit_product(x);
                    ++steps;
                }

                if (steps == 6 && x < 10)
                {
                    /* Found a valid term */
                    ensure_seq_capacity(seq_count + 1);
                    seq[seq_count++] = (seq_t)next_candidate;
                    ++next_candidate;   /* move past this candidate */
                    break;              /* exit inner loop */
                }
            }
            ++next_candidate;           /* try the next integer */
        }
    }

    return seq[(size_t)n - 1];       /* 1‑based indexing */
}
