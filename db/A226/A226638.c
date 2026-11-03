/*
 * A226638  Product of Pell and Lucas numbers.
 * https://oeis.org/A226638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n) * Lucas(n) Pell: P(0)=0, P(1)=1,
 *            P(n)=2*P(n-1)+P(n-2) Lucas: L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    static seq_t *P = NULL;
    static seq_t *L = NULL;
    static size_t p_cap = 0;
    static size_t l_cap = 0;
    static size_t computed = 0;

    /* initialize the first two terms once */
    if (computed == 0) {
        ensure_capacity(2, &p_cap, OEISC_VPP(&P), sizeof(seq_t));
        ensure_capacity(2, &l_cap, OEISC_VPP(&L), sizeof(seq_t));
        P[0] = (seq_t)0;
        P[1] = (seq_t)1;
        L[0] = (seq_t)2;
        L[1] = (seq_t)1;
        computed = 2;
    }

    /* extend to n if necessary */
    if ((size_t)n >= computed) {
        size_t new_cap = (size_t)n + 1;
        ensure_capacity(new_cap, &p_cap, OEISC_VPP(&P), sizeof(seq_t));
        ensure_capacity(new_cap, &l_cap, OEISC_VPP(&L), sizeof(seq_t));

        for (size_t i = computed; i <= (size_t)n; ++i) {
            /* Pell recurrence: P[i] = 2*P[i-1] + P[i-2] */
            seq_t two_pi_minus1 = mul_small(P[i-1], 2);
            P[i] = two_pi_minus1 + P[i-2];

            /* Lucas recurrence: L[i] = L[i-1] + L[i-2] */
            L[i] = L[i-1] + L[i-2];
        }
        computed = new_cap;
    }

    return seq_mul(P[(size_t)n], L[(size_t)n]);
}
