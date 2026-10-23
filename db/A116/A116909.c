/*
 * A116909  Start with the sequence 2322322323222323223223 and extend by
 *          always appending the curling number (cf. A094004).
 * https://oeis.org/A116909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 500, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Curling number sequence starting from the seed
   2,3,2,2,3,2,2,3,2,3,2,2,2,3,2,3,2,2,3,2,2,3,2.
   After the seed the sequence is extended by repeatedly appending the curling
   number of the entire current sequence.  The curling number of a sequence
   S is the largest integer k such that the last k elements of S can be
   partitioned into k consecutive identical blocks.  */

#include <stdint.h>
#include <stdlib.h>

static seq_t *seq     = NULL;   /* dynamically grown array of terms */
static size_t seq_len = 0;      /* number of terms currently stored */
static size_t seq_cap = 0;      /* current capacity of the array */

/* Ensure the array has capacity for at least `need` elements. */
static void ensure_cap(size_t need)
{
    if (need <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 32;
    while (new_cap < need)
        new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Compute the curling number of the first `len` elements of `seq`. */
static seq_t compute_curling(size_t len)
{
    /* The curling number is at least 1. */
    size_t best = 1;
    for (size_t d = 1; d <= len; ++d) {
        size_t pos = len;    /* position just after the last element */
        size_t k   = 0;     /* number of additional copies beyond the first */

        /* Count how many identical blocks of length `d` end the sequence. */
        while (pos >= 2 * d) {
            size_t i1 = pos - 2 * d;   /* start of the preceding block   */
            size_t i2 = pos - d;       /* start of the trailing block    */
            int same = 1;
            for (size_t j = 0; j < d; ++j) {
                if (seq[i1 + j] != seq[i2 + j]) {
                    same = 0;
                    break;
                }
            }
            if (!same) break;
            ++k;
            pos -= d;
        }
        size_t count = k + 1;
        if (count > best)
            best = count;
    }
    return (seq_t)best;
}

/* Initialize the sequence with the fixed seed on the first call. */
static void init_seed(void)
{
    if (seq != NULL)
        return;
    static const seq_t seed_series[] = {
        2,3,2,2,3,2,2,3,2,3,2,2,2,3,2,3,2,2,3,2,2,3,2
    };
    size_t seed_len = sizeof(seed_series)/sizeof(seed_series[0]);
    ensure_cap(seed_len);
    for (size_t i = 0; i < seed_len; ++i)
        seq[i] = seed_series[i];
    seq_len = seed_len;
    seq_cap = seed_len;
}

/* *************************************************************** */
/* The required public function                                         */
/* *************************************************************** */
seq_t a(idx_t n)
{
    if (n == 0)                 /* sequence is 1‑based; caller may ignore 0 */
        return (seq_t)0;        /* arbitrary value */
    init_seed();
    /* Extend the sequence up to index n (1‑based). */
    while (seq_len < (size_t)n) {
        seq_t curl = compute_curling(seq_len);
        ensure_cap(seq_len + 1);
        seq[seq_len] = curl;
        ++seq_len;
    }
    return seq[(size_t)n - 1];
}
