/*
 * A105483  Number of partitions of {1...n} containing one string of 3
 *          consecutive integers, counted within a block.
 * https://oeis.org/A105483
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A105483
 *  a(n) = number of partitions of {1..n} that contain exactly one
 *  string of three consecutive integers.
 *
 *  The implementation follows the recurrence for c(n,k,r) where
 *  c(n,k,0) = c(n-1,k-1,0)+(k-1)c(n-1,k,0)+c(n-2,k-1,0)+(k-1)c(n-2,k,0),
 *  c(n,k,1) = c(n-1,k-1,1)+(k-1)c(n-1,k,1)+c(n-2,k-1,1)+(k-1)c(n-2,k,1)
 *              +c(n-1,k,0)-c(n-2,k-1,0)-(k-1)c(n-2,k,0).
 *  Then a(n)=Σ_k c(n,k,1).  The routine memoises results so any
 *  call order is acceptable.  It runs in O(n²) time and uses only
 *  O(n) memory.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Allocate or grow all arrays to hold indices 0..new_cap.
 * Newly added elements are zeroed. */
static void ensure_capacity(idx_t new_cap,
                            seq_t **c0_prev1, seq_t **c0_prev2, seq_t **c0_cur,
                            seq_t **c1_prev1, seq_t **c1_prev2, seq_t **c1_cur,
                            seq_t **aval, idx_t *capacity)
{
    if (new_cap <= *capacity) return;

    size_t sz = (size_t)(new_cap + 1) * sizeof(seq_t);

    *c0_prev1 = (seq_t *)realloc(*c0_prev1, sz);
    *c0_prev2 = (seq_t *)realloc(*c0_prev2, sz);
    *c0_cur   = (seq_t *)realloc(*c0_cur,   sz);
    *c1_prev1 = (seq_t *)realloc(*c1_prev1, sz);
    *c1_prev2 = (seq_t *)realloc(*c1_prev2, sz);
    *c1_cur   = (seq_t *)realloc(*c1_cur,   sz);
    *aval     = (seq_t *)realloc(*aval,     sz);

    if (!*c0_prev1 || !*c0_prev2 || !*c0_cur ||
        !*c1_prev1 || !*c1_prev2 || !*c1_cur ||
        !*aval)
        exit(EXIT_FAILURE);

    /* zero initialise the newly added entries */
    for (idx_t i = *capacity + 1; i <= new_cap; ++i) {
        (*c0_prev1)[i] = 0;
        (*c0_prev2)[i] = 0;
        (*c0_cur  )[i] = 0;
        (*c1_prev1)[i] = 0;
        (*c1_prev2)[i] = 0;
        (*c1_cur  )[i] = 0;
        (*aval    )[i] = 0;
    }
    *capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* static state shared between all calls */
    static idx_t    capacity   = 0;   /* maximum index for which arrays are allocated */
    static idx_t    computed   = 0;   /* largest n for which a[n] is known */
    static seq_t  *c0_prev1 = NULL, *c0_prev2 = NULL, *c0_cur   = NULL;
    static seq_t  *c1_prev1 = NULL, *c1_prev2 = NULL, *c1_cur   = NULL;
    static seq_t  *aval     = NULL;

    /* initialise on first call */
    if (capacity == 0) {
        /* start with capacity 2 to avoid frequent reallocs */
        ensure_capacity(2,
                        &c0_prev1, &c0_prev2, &c0_cur,
                        &c1_prev1, &c1_prev2, &c1_cur,
                        &aval, &capacity);

        /* row n = 0 */
        c0_prev1[0] = 1;          /* c(0,0,0) = 1 */
        /* other entries are zero */
        aval[0] = 0;
        aval[1] = 0;
        aval[2] = 0;
        computed = 0;
    }

    /* grow tables if necessary */
    if (n > capacity)
        ensure_capacity(n,
                        &c0_prev1, &c0_prev2, &c0_cur,
                        &c1_prev1, &c1_prev2, &c1_cur,
                        &aval, &capacity);

    /* build rows up to requested n */
    while (computed < n) {
        idx_t new_n = computed + 1;   /* next n to compute */

        /* compute c(*,*,0) for n = new_n */
        for (idx_t k = 0; k <= new_n; ++k) {
            seq_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;

            if (k >= 1) {
                /* terms from n-1 row */
                t1 = c0_prev1[k-1];
                t2 = c0_prev1[k] * (seq_t)(k-1);

                /* terms from n-2 row, if available */
                if (new_n >= 2) {
                    t3 = c0_prev2[k-1];
                    t4 = c0_prev2[k] * (seq_t)(k-1);
                }
            }
            c0_cur[k] = t1 + t2 + t3 + t4;
        }

        /* compute c(*,*,1) for n = new_n */
        for (idx_t k = 0; k <= new_n; ++k) {
            seq_t part = 0;
            if (k >= 1) {
                part = c1_prev1[k-1];
                part += c1_prev1[k] * (seq_t)(k-1);
                part += c1_prev2[k-1];
                part += c1_prev2[k] * (seq_t)(k-1);
                part += c0_prev1[k];
            }

            seq_t sub = 0;
            if (new_n >= 2 && k >= 1) {
                sub = c0_prev2[k-1];
                sub += c0_prev2[k] * (seq_t)(k-1);
            }

            c1_cur[k] = part - sub;   /* guaranteed non‑negative */
        }

        /* compute a(new_n) = sum_k c(new_n,k,1) */
        seq_t sum = 0;
        for (idx_t k = 0; k <= new_n; ++k)
            sum += c1_cur[k];
        aval[new_n] = sum;

        /* rotate row buffers:
           prev2 <- prev1, prev1 <- cur, cur <- old prev2 */
        seq_t *tmp;

        tmp = c0_prev2; c0_prev2 = c0_prev1; c0_prev1 = c0_cur; c0_cur = tmp;
        tmp = c1_prev2; c1_prev2 = c1_prev1; c1_prev1 = c1_cur; c1_cur = tmp;

        computed = new_n;
    }

    return aval[n];
}
