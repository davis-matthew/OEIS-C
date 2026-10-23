/*
 * A043769  Numbers k such that number of runs in the base 2 representation of
 *          k is congruent to 6 mod 10.
 * https://oeis.org/A043769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers k such that the number of runs in the binary
 *  representation of k is congruent to 6 (mod 10).
 *
 *  The function builds the sequence lazily: it keeps a static
 *  array of the computed terms and extends it whenever a
 *  larger index is requested.  For each candidate integer
 *  it counts the runs of consecutive equal bits by right
 *  shifting until the value becomes zero.
 *
 *  Complexity: O(n * L) where L is the bit‑length of the
 *  candidates (at most a few dozen for the tested range).
 *  For n up to about 10 000 the program finishes far faster
 *  than the 2.0‑second limit.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Count the number of runs of identical bits in the binary
 * representation of x.  The function works for any unsigned
 * integer type seq_t provided in this build.
 */
static inline int count_runs(seq_t x)
{
    if (x == 0)            /* 0 is not used, but keep definition safe */
        return 1;
    int runs = 1;
    seq_t last_bit = x & 1;   /* least significant bit */
    x >>= 1;
    while (x) {
        seq_t bit = x & 1;
        if (bit != last_bit) {
            ++runs;
            last_bit = bit;
        }
        x >>= 1;
    }
    return runs;
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* static state – only constructed once, reused on subsequent calls */
    static seq_t *seq = NULL;     /* array of stored terms */
    static size_t sz  = 0;        /* number of terms currently stored */
    static size_t cap = 0;        /* current capacity of 'seq' */
    static seq_t cand = 0;        /* last candidate examined */

    if (n == 0)          /* offset is 1; index 0 is unused */
        return 0;

    /* Grow the array until it contains at least n terms. */
    while (sz < n) {
        ++cand;                              /* next candidate value */
        if (count_runs(cand) % 10 == 6) {    /* satisfies the condition ? */
            if (sz == cap) {                /* need more space */
                size_t newcap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                if (!tmp) abort();          /* out of memory */
                seq = tmp;
                cap = newcap;
            }
            seq[sz++] = cand;                /* store the term */
        }
    }
    return seq[n - 1];
}
