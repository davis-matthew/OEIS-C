/*
 * A099591  Numbers that are the sum of no fewer than 17 biquadrates (4th
 *          powers).
 * https://oeis.org/A099591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A099591 – Numbers that are the sum of no fewer than 17 biquadrates.
   a(n) is the nth member of that sequence.  The term is computed by
   determining, for each integer s up to the largest known member
   (13792), the minimal number of 4th powers whose sum is s.
   Those s with a minimal count > 16 form the sequence.  The list
   contains 96 terms; for n greater than that a value of 0 is returned.  */

#include <stdlib.h>
#include <stdint.h>

/*--- cached data ----------------------------------------------------- */
static unsigned short *dp     = NULL;   /* minimal number of 4th powers */
static size_t         dp_size = 0;
static seq_t       *seq_vals = NULL;     /* sequence terms */
static size_t         seq_len = 0, seq_cap = 0;

/*--- initialise tables (once) ---------------------------------------- */
static void init(void)
{
    if (dp != NULL)                 /* already initialised */
        return;

    /* bound of the known sequence (last term is 13792) */
    const size_t max_val = 13792;
    dp_size = max_val + 1;

    dp = (unsigned short *)malloc(dp_size * sizeof *dp);
    if (dp == NULL)
        return;                     /* allocation failed – leave uninitialised */

    /* coins: all fourth powers <= max_val */
    unsigned pow[30];
    size_t pcnt = 0;
    for (unsigned x = 1; ; ++x) {
        uint64_t p = (uint64_t)x * x * x * x;
        if (p > max_val)
            break;
        pow[pcnt++] = (unsigned)p;
    }

    /* coin‑change DP for minimal number of terms */
    const unsigned short INF = 20000;          /* larger than any realistic count */
    for (size_t i = 0; i < dp_size; ++i)
        dp[i] = INF;
    dp[0] = 0;

    for (size_t s = 1; s < dp_size; ++s) {
        unsigned short best = INF;
        for (size_t i = 0; i < pcnt && pow[i] <= s; ++i) {
            unsigned short cand = dp[s - pow[i]];
            if (cand < INF && cand + 1 < best)
                best = cand + 1;
        }
        dp[s] = best;
    }

    /* collect all numbers that need at least 17 4th powers */
    seq_cap = 128;                       /* initial capacity */
    seq_vals = (seq_t *)malloc(seq_cap * sizeof *seq_vals);
    if (seq_vals == NULL) {
        free(dp);
        dp = NULL;
        return;
    }

    for (size_t s = 1; s < dp_size; ++s) {
        if (dp[s] > 16) {                /* minimal count > 16 */
            if (seq_len >= seq_cap) {
                seq_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(seq_vals, seq_cap * sizeof *seq_vals);
                if (tmp == NULL) {        /* allocation failed */
                    free(dp);
                    free(seq_vals);
                    dp = NULL;
                    seq_vals = NULL;
                    seq_len = 0;
                    seq_cap = 0;
                    return;
                }
                seq_vals = tmp;
            }
            seq_vals[seq_len++] = (seq_t)s;
        }
    }
}

/*--- sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    init();                               /* ensure tables are ready */

    if (n == 0)
        return (seq_t)0;                  /* sequence is 1‑based */

    if (seq_vals == NULL)
        return (seq_t)0;                  /* uninitialised / bad state */

    if ((size_t)n <= seq_len)
        return seq_vals[(size_t)n - 1];

    /* beyond known terms – return 0 as per specifications */
    return (seq_t)0;
}
