/*
 * A109331  Integers with mutual residues of 6 or more.
 * https://oeis.org/A109331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A109331: Integers with mutual residues of 6 or more.
   Definition: a(1)=7; for n>1, a(n)=min{ m | m>a(n-1) and
   for all i<n, m mod a(i) ≥ 6 }.
   The function memoises computed terms and works for any increasing or
   non‑monotonic sequence of calls.  It is written to be valid C11 and
   C++17; for C++ it uses vector‑style allocation with new[]/delete[],
   for C it falls back to malloc/realloc.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>      /* for uint64_t */

static seq_t *seq = NULL;        /* array of previously computed terms */
static size_t seq_cap = 0;       /* capacity of seq array              */
static size_t seq_len = 0;       /* number of terms currently stored   */

/* Allocate or grow the dynamic array to hold at least new_cap elements.
   Uses new/delete[] in C++ and malloc/realloc/free in C.  */
static void ensure_cap(size_t new_cap) {
    if (seq_cap < new_cap) {
        size_t better = seq_cap ? seq_cap * 2 : 4;
        if (better < new_cap) better = new_cap;

#ifdef __cplusplus
        seq_t* new_arr = new seq_t[better];
        for (size_t i = 0; i < seq_len; ++i) new_arr[i] = seq[i];
        delete[] seq;
#else
        seq_t* new_arr = (seq_t*)realloc(seq, better * sizeof(seq_t));
        /* realloc keeps the existing content; we don't need to copy */
#endif
        seq = new_arr;
        seq_cap = better;
    }
}

/* Append one new value to the cached sequence.  */
static void push_seq(seq_t val) {
    ensure_cap(seq_len + 1);
    seq[seq_len++] = val;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;     /* undefined for n<1, return 0 as safe placeholder */

    /* initialise the first term if the cache is empty */
    if (seq_len == 0) push_seq((seq_t)7);

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        seq_t prev = seq[seq_len - 1];
        seq_t m = prev + (seq_t)1;            /* start just after the last term */

        /* search for the minimal m satisfying all residues ≥ 6 */
        while (true) {
            bool ok = true;
            for (size_t i = 0; i < seq_len; ++i) {
                seq_t r = m % seq[i];
                if (r < (seq_t)6) {           /* fail – need to jump */
                    m += ((seq_t)6) - r;
                    ok = false;
                    break;                   /* restart checking from scratch */
                }
            }
            if (ok) break;                    /* all residues are ≥ 6 */
        }
        push_seq(m);
    }
    return seq[n - 1];
}
