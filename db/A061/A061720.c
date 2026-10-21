/*
 * A061720  First differences of sequence of primorials.
 * https://oeis.org/A061720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002110(n+1) – A002110(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   _next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_next_prime.h"

/* ---------- sequence a(n) ----------
 * Computes a(n) = primorial(n+1) – primorial(n).
 * Cache primorials and a(n) statically to support arbitrary call order.
 */
seq_t a(idx_t n) {
    static seq_t *primorial = NULL;  /* primorial[0] = 1, primorial[1] = 2, ... */
    static size_t prim_size = 0, prim_cap = 0;

    static seq_t *a_arr = NULL;      /* a_arr[0] = a(0), a_arr[1] = a(1), ... */
    static size_t a_size = 0, a_cap = 0;

    /* one‑time initialization */
    if (primorial == NULL) {
        prim_cap = 8;
        primorial = (seq_t *)malloc(prim_cap * sizeof(seq_t));
        primorial[0] = (seq_t)1;          /* primorial(0) = 1 */
        prim_size = 1;

        a_cap = 8;
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_size = 0;
    }

    /* Generate primorials up to index n+1, and the corresponding a(n) */
    while (prim_size <= (size_t)n + 1) {
        uint32_t p = _next_prime();           /* next prime */
        seq_t prev = primorial[prim_size - 1];
        seq_t cur  = prev * (seq_t)p;         /* new primorial */

        /* grow primorial array if needed */
        if (prim_size == prim_cap) {
            prim_cap <<= 1;
            primorial = (seq_t *)realloc(primorial,
                                         prim_cap * sizeof(seq_t));
        }
        primorial[prim_size++] = cur;

        /* compute a[prim_size-2] = cur – prev */
        if (a_size == a_cap) {
            a_cap <<= 1;
            a_arr = (seq_t *)realloc(a_arr,
                                     a_cap * sizeof(seq_t));
        }
        a_arr[a_size++] = cur - prev;
    }

    return a_arr[(size_t)n];
}
