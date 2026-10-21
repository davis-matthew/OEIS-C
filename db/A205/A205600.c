/*
 * A205600  Start with first run [1,2] then, for n >= 2, the n-th run is a(n)
 *          times [a(1),...,a(n-1)].
 * https://oeis.org/A205600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205600: a(1)=1, a(2)=2.  
   For n>=2 the n-th run is a(n) copies of the prefix [a(1),...,a(n-1)]. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* static state shared by all calls */
static seq_t *seq = NULL;          /* 1‑indexed array of terms               */
static size_t seq_cap = 0;         /* current capacity (excluding index 0)   */
static idx_t seq_len = 0;          /* number of stored terms                 */
static idx_t next_run = 2;         /* next run index to be processed        */

/* enlarge seq so that it can hold at least 'need' indices (1‑based) */
static void grow_seq(size_t need)
{
    if (seq_cap >= need) return;
    size_t newcap = seq_cap ? seq_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *newptr = (seq_t *)realloc(seq, (newcap + 1) * sizeof(seq_t));
    if (!newptr) abort();               /* out of memory, terminate */
    seq = newptr;
    seq_cap = newcap;
}

/* --------------------------------------------------------------- */
/*  Return the n-th term of A205600 (1‑based index).               */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* behaviour below offset is irrelevant */

    /* lazy initialization of the sequence */
    if (!seq) {
        grow_seq(16);                     /* initial capacity (index 1..16)    */
        seq[1] = 1;
        seq[2] = 2;
        seq_len = 2;
        next_run = 2;                     /* process the second run next      */
    }

    /* generate terms until we have at least n terms */
    while (seq_len < n) {
        /* multiplicity for the current run */
        seq_t k_seq = seq[next_run];
        idx_t copies = (idx_t)k_seq;          /* runs are small for n<=10000   */

        /* append 'copies' copies of the prefix [1 .. next_run-1] */
        for (idx_t c = 0; c < copies; ++c) {
            for (idx_t i = 1; i < next_run; ++i) {
                ++seq_len;
                if (seq_len > seq_cap) grow_seq(seq_len);
                seq[seq_len] = seq[i];
            }
        }
        ++next_run;                     /* advance to the next run          */
    }

    return seq[n];
}
