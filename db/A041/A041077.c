/*
 * A041077  Denominators of continued fraction convergents to sqrt(45).
 * https://oeis.org/A041077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A041077: Denominators of continued‑fraction convergents to sqrt(45)
   Recurrence: a(n) = 322*a(n-6) - a(n-12)   for n ≥ 12
   Initial values (n = 0 … 11):
   1, 1, 3, 7, 17, 24, 305, 329, 963, 2255,
   5473, 7728
*/

#include <stdlib.h>
#include <stddef.h>

/* ----- storage of the sequence ----------------------------------- */
static seq_t *seq = NULL;   /* dynamic array of sequence values   */
static size_t cap  = 0;     /* current allocated capacity        */
static idx_t  last = (idx_t)-1;   /* last index that has a value */

/* ----- helper: allocate/resize the array -------------------------- */
static void seq_allocate(size_t new_cap)
{
#ifdef __cplusplus
    seq_t *new_seq = new seq_t[new_cap];
    if (seq) {
        for (size_t i = 0; i < cap; ++i) new_seq[i] = seq[i];
        delete [] seq;
    }
    seq = new_seq;
#else
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
#endif
    cap = new_cap;
    /* initialise new cells to zero (safe for both C and C++) */
    for (size_t i = 0; i < cap; ++i)
        if (i >= cap - (new_cap - cap)) seq[i] = (seq_t)0;
}

/* ----- initialise with known first 12 values ---------------------- */
static void seq_init(void)
{
    const size_t init_cap = 16;  /* start with a small power of two */
    seq_allocate(init_cap);

    const seq_t init_vals[12] = {
        (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)7,
        (seq_t)17, (seq_t)24, (seq_t)305, (seq_t)329,
        (seq_t)963, (seq_t)2255, (seq_t)5473, (seq_t)7728
    };

    for (size_t i = 0; i < 12; ++i) seq[i] = init_vals[i];
    for (size_t i = 12; i < cap; ++i) seq[i] = (seq_t)0;

    last = 11;
}

/* ----- ensure array can hold up to index n ----------------------- */
static void seq_ensure(idx_t n)
{
    if (seq && n < (idx_t)(cap)) return;

    size_t new_cap = cap ? cap : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_allocate(new_cap);
}

/* ----- compute values up to n (inclusive) ------------------------ */
static void seq_compute(idx_t n)
{
    if (!seq) seq_init();          /* lazy initialisation */
    if (n <= last) return;        /* already computed */

    seq_ensure(n);

    for (idx_t i = last + 1; i <= n; ++i) {
        if (i < 12) continue;     /* first 12 terms already set */
        seq[i] = (seq_t)322 * seq[i - 6] - seq[i - 12];
    }
    last = n;
}

/* ----- public function --------------------------------------------- */
seq_t a(idx_t n)
{
    seq_compute(n);
    return seq[n];
}
