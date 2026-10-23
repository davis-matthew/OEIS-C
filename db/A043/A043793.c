/*
 * A043793  Numbers whose number of runs in base 3 is congruent to 2 (mod 7).
 * https://oeis.org/A043793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043793: numbers whose number of runs in base 3 is ≡ 2 (mod 7) */

#include <stdlib.h>

#if defined(__cplusplus)
/* Use C++ new/delete for dynamic allocation */
#include <new>
#define SEQ_ALLOC(type, n) new type[n]
#define SEQ_DEALLOC(ptr) delete[] ptr
#else
/* Use C malloc/free */
#define SEQ_ALLOC(type, n) ((type*)malloc((n)*sizeof(type)))
#define SEQ_DEALLOC(ptr) free(ptr)
#endif

/* Count the number of runs in the base‑3 representation of x.
   A run is a maximal block of equal consecutive digits. */
static inline int runs_in_base3(seq_t x)
{
    int runs = 0;
    int prev = -1;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)3);
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x /= (seq_t)3;
    }
    return runs;
}

/* Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* undefined for n < 1 */

    /* Static cache of previously found terms */
    static seq_t *seq = NULL;
    static size_t sz = 0, cap = 0;

    if ((size_t)n <= sz) {
        return seq[n - 1];
    }

    /* Start searching from the next integer after the last cached term */
    seq_t cur = sz ? seq[sz - 1] + (seq_t)1 : (seq_t)1;

    for (;;) {
        if (runs_in_base3(cur) % 7 == 2) {
            /* Grow the cache if necessary */
            if (sz == cap) {
                size_t new_cap = cap == 0 ? 1 : cap * 2;
                seq_t *tmp = SEQ_ALLOC(seq_t, new_cap);
                if (tmp == NULL) abort();   /* out of memory */

                /* Copy existing terms into the new buffer */
                for (size_t i = 0; i < sz; ++i) {
                    tmp[i] = seq[i];
                }

                /* Release old buffer */
                if (seq != NULL) SEQ_DEALLOC(seq);
                seq = tmp;
                cap = new_cap;
            }

            seq[sz++] = cur;
            if ((size_t)n == sz) break;
        }
        ++cur;
    }

    return seq[n - 1];
}
