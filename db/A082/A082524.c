/*
 * A082524  a(1)=1, a(2)=2, then use the rule when a(n) is the end of a run, n
 *          appears a(n) times.
 * https://oeis.org/A082524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n≥2, after a run of value v ends at index n,
 *            the value n appears v times. Thus each run has length equal to
 *            its value, and the next run starts with the index of the
 *            previous run’s end and has the same length as the previous run’s
 *            value
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static idx_t   seq_len = 0;     /* number of terms computed (last index) */
static size_t  seq_cap = 0;     /* allocated capacity (number of indices, 1‑based) */
static seq_t * seq    = NULL;   /* array holding a(1)…a(seq_len) */
static seq_t   cur_val = 0;     /* current run value */
static idx_t   remaining = 0;   /* occurrences of cur_val still to output */

/* Initialise the sequence with the two seed values and the first run state. */
static void init_seq(void)
{
    if (seq_len != 0) return;           /* already initialised */

    seq_cap = 16;
    seq = (seq_t *)malloc((seq_cap + 1) * sizeof(seq_t));   /* index 0 unused */
    if (!seq) abort();

    seq[1] = 1;          /* a(1) */
    seq[2] = 2;          /* a(2) */
    seq_len = 2;

    cur_val    = 2;      /* first run value (value 2) */
    remaining  = 1;      /* a[2] - 1 : one more 2 to output (index 3) */
}

/* Grow the stored sequence up to (and including) index n. */
static void grow_to(idx_t n)
{
    init_seq();

    while (seq_len < n) {
        /* Ensure enough capacity */
        if (seq_len + 1 > seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 16;
            while (new_cap < seq_len + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));
            if (!tmp) abort();
            seq = tmp;
            seq_cap = new_cap;
        }

        idx_t next_index = seq_len + 1;
        seq[next_index] = cur_val;
        seq_len = next_index;

        /* Consume one occurrence of the current run. */
        remaining--;

        if (remaining == 0) {            /* run ended at next_index */
            idx_t run_end  = seq_len;    /* n */
            seq_t prev_val = cur_val;    /* a(n) */
            cur_val     = (seq_t)run_end;
            remaining   = (idx_t)prev_val;   /* new run length */
        }
    }
}

/* Public function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but safe for n<1 */
    grow_to(n);
    return seq[n];
}
