/*
 * A043610  Numbers whose base-6 representation has exactly 2 runs.
 * https://oeis.org/A043610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑6 representation has exactly 2 runs.
   a(n) = nth smallest positive integer whose base‑6
   digit sequence consists of exactly two maximal runs
   of equal digits. */

/* Uses a max‑heap of size n to keep the n smallest values
   without sorting the entire list. */

#include <stddef.h>
#include <stdlib.h>

/* heap swap helper */
static inline void heap_swap(seq_t *a, seq_t *b) {
    seq_t tmp = *a;
    *a = *b;
    *b = tmp;
}

seq_t a(idx_t n) {
    if (n < 1)
        return 0;   /* the sequence is undefined for n < 1 */

    /* ------------------------------------------------------------------ */
    /* 1. Find the maximum string length needed to produce at least n values */
    /* ------------------------------------------------------------------ */
    idx_t total = 0;
    idx_t len = 2;
    while (total < n) {
        total += 25 * (len - 1);   /* 5 choices for d * 5 for e != d * (len-1) splits */
        ++len;
    }
    idx_t max_len = len - 1;   /* inclusive maximum length */

    /* ------------------------------------------------------------------ */
    /* 2. Pre‑compute powers of 6 up to max_len */
    /* ------------------------------------------------------------------ */
#ifdef __cplusplus
    /* In C++ the seq_t type handles its own size (could be an arbitrary‑precision class) */
    seq_t *pow6 = new seq_t[max_len + 1];
    pow6[0] = 1;
    for (idx_t i = 1; i <= max_len; ++i)
        pow6[i] = pow6[i - 1] * 6;
#else
    /* In pure C use unsigned __int128 for intermediate values to detect overflow */
    unsigned __int128 *pow6 = (unsigned __int128 *)malloc((max_len + 1) * sizeof(unsigned __int128));
    pow6[0] = 1;
    for (idx_t i = 1; i <= max_len; ++i)
        pow6[i] = pow6[i - 1] * 6;
#endif

    /* ------------------------------------------------------------------ */
    /* 3. Allocate the max‑heap of size n */
    /* ------------------------------------------------------------------ */
#if defined(__cplusplus)
    seq_t *heap = new seq_t[n];
#else
    seq_t *heap = (seq_t *)malloc(n * sizeof(seq_t));
#endif
    size_t heap_size = 0;

    /* ------------------------------------------------------------------ */
    /* 4. Generate all numbers with two runs up to max_len digits */
    /* ------------------------------------------------------------------ */
    for (idx_t P = 2; P <= max_len; ++P) {              /* total length = P */
        for (idx_t L = 1; L <= P - 1; ++L) {            /* length of first run  */
            idx_t M = P - L;                            /* length of second run */

#ifdef __cplusplus
            seq_t prefix = (pow6[L] - 1) / 5;           /* (6^L - 1)/5 */
            seq_t suffix = (pow6[M] - 1) / 5;           /* (6^M - 1)/5 */
#else
            unsigned __int128 prefix = (pow6[L] - 1) / 5;
            unsigned __int128 suffix = (pow6[M] - 1) / 5;
#endif

            for (int d = 1; d <= 5; ++d) {
                for (int e = 0; e <= 5; ++e) {
                    if (e == d) continue;

#ifdef __cplusplus
                    seq_t val = ((seq_t)d) * prefix * pow6[M] + ((seq_t)e) * suffix;
#else
                    /* Compute with 128‑bit to detect overflow */
                    unsigned __int128 val128 = (unsigned __int128)d * prefix * pow6[M]
                                                 + (unsigned __int128)e * suffix;
                    if (val128 > (unsigned __int128)((seq_t)-1))
                        continue;   /* value does not fit in seq_t */
                    seq_t val = (seq_t)val128;
#endif
                    /* Insert value into the max‑heap of size n */
                    if (heap_size < (size_t)n) {
                        heap[heap_size++] = val;
                        /* bubble up */
                        size_t i = heap_size - 1;
                        while (i > 0) {
                            size_t parent = (i - 1) >> 1;
                            if (heap[parent] >= heap[i]) break;
                            heap_swap(&heap[parent], &heap[i]);
                            i = parent;
                        }
                    } else if (val < heap[0]) {
                        heap[0] = val;
                        /* bubble down */
                        size_t i = 0;
                        for (;;) {
                            size_t left = (i << 1) + 1;
                            size_t right = left + 1;
                            size_t largest = i;
                            if (left < heap_size && heap[left] > heap[largest])
                                largest = left;
                            if (right < heap_size && heap[right] > heap[largest])
                                largest = right;
                            if (largest == i) break;
                            heap_swap(&heap[i], &heap[largest]);
                            i = largest;
                        }
                    }
                }
            }
        }
    }

    /* ------------------------------------------------------------------ */
    /* 5. Result is the smallest value in the heap (root of max‑heap) */
    /* ------------------------------------------------------------------ */
    seq_t result = heap[0];

    /* ------------------------------------------------------------------ */
    /* 6. Clean up */
    /* ------------------------------------------------------------------ */
#if defined(__cplusplus)
    delete[] heap;
    delete[] pow6;
#else
    free(heap);
    free(pow6);
#endif

    return result;
}
