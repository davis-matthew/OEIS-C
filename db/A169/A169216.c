/*
 * A169216  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=11, a(n)=11*10^(n-1) for 2≤n≤27;
 *            a(28)=9*∑_{i=1}^{27}a(i)−45*a(0)+1; for n≥29:
 *            a(n)=9*∑_{i=n-27}^{n-1}a(i)−45*a(n-28)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static idx_t size = 0;
    static idx_t cap = 0;
    static seq_t sum27 = 0;      /* sum of the last 27 values */

    if (size <= n) {
        ensure_capacity((size_t)(n + 1), &cap, OEISC_VPP(&seq), sizeof(seq_t));

        for (idx_t i = size; i <= n; ++i) {
            if (i == 0) {
                seq[0] = (seq_t)1;
            } else if (i <= 27) {
                seq[i] = seq_mul((seq_t)11, pow10(i - 1));
            } else if (i == 28) {
                /* compute sum of a[1..27] */
                sum27 = (seq_t)0;
                for (idx_t j = 1; j <= 27; ++j)
                    sum27 = sum27 + seq[j];
                seq[28] = seq_mul((seq_t)9, sum27);
                seq[28] = seq[28] - seq_mul((seq_t)45, seq[0]) + (seq_t)1;
                /* update sum27 for next step: sum of a[2..28] */
                sum27 = sum27 + seq[28] - seq[1];
            } else { /* i > 28 */
                seq[i] = seq_mul((seq_t)9, sum27);
                seq[i] = seq[i] - seq_mul((seq_t)45, seq[i - 28]);
                /* update sum27: now sum of a[i-26 .. i] */
                sum27 = sum27 + seq[i] - seq[i - 27];
            }
        }
        size = n + 1;
    }
    return seq[n];
}
