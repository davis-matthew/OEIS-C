/*
 * A112759  Total number of prime factors of n-th 5-smooth number.
 * https://oeis.org/A112759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = total number of prime factors (with multiplicity) of the
 *            n-th 5‑smooth number (product of 2^a 3^b 5^c)
 * Range:     u8 n <= 50, u16 n <= 615, u32 n <= 1982, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Returns the total number of prime factors of the n-th 5‑smooth number. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* undefined offset */
    /* Static state to keep the generated 5‑smooth numbers across calls */
    static idx_t size = 0;                    /* number of values stored (>= n) */
    static idx_t cap = 0;                     /* capacity of allocated arrays    */
    static seq_t *vals = NULL;                /* values of 5‑smooth numbers      */
    static idx_t *e2 = NULL;                  /* exponents of 2                  */
    static idx_t *e3 = NULL;                  /* exponents of 3                  */
    static idx_t *e5 = NULL;                  /* exponents of 5                  */

    /* ---- allocation/on‑demand growth ---- */
    if (cap == 0) {                           /* first time */
        cap = 1024;
        vals = (seq_t*)malloc(sizeof(seq_t) * (cap + 1));
        e2   = (idx_t *)malloc(sizeof(idx_t) * (cap + 1));
        e3   = (idx_t *)malloc(sizeof(idx_t) * (cap + 1));
        e5   = (idx_t *)malloc(sizeof(idx_t) * (cap + 1));
        if (!vals || !e2 || !e3 || !e5) { return 0; } /* out of memory */
        /* index 1 holds 1 (the first 5‑smooth number) */
        vals[1] = 1;
        e2[1] = e3[1] = e5[1] = 0;
        size   = 1;
    }
    /* ---- generate until we have at least n values ---- */
    static idx_t i2 = 1, i3 = 1, i5 = 1;       /* pointers for next candidates */
    while (size < n) {
        seq_t v2 = vals[i2] * (seq_t)2;
        seq_t v3 = vals[i3] * (seq_t)3;
        seq_t v5 = vals[i5] * (seq_t)5;
        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;

        /* determine exponents for the new entry */
        idx_t ex2 = 0, ex3 = 0, ex5 = 0;
        if (next == v2) { ex2 = e2[i2] + 1; ex3 = e3[i2]; ex5 = e5[i2]; }
        if (next == v3) { ex2 = e2[i3];     ex3 = e3[i3] + 1; ex5 = e5[i3]; }
        if (next == v5) { ex2 = e2[i5];     ex3 = e3[i5];     ex5 = e5[i5] + 1; }

        /* grow buffers if needed */
        if (size + 1 >= cap) {
            idx_t new_cap = cap * 2;
            seq_t *new_vals = (seq_t*)realloc(vals, sizeof(seq_t) * (new_cap + 1));
            if (!new_vals) return 0;   /* allocation failed */
            vals = new_vals;
            idx_t *new_e2 = (idx_t*)realloc(e2, sizeof(idx_t) * (new_cap + 1));
            idx_t *new_e3 = (idx_t*)realloc(e3, sizeof(idx_t) * (new_cap + 1));
            idx_t *new_e5 = (idx_t*)realloc(e5, sizeof(idx_t) * (new_cap + 1));
            if (!new_e2 || !new_e3 || !new_e5) return 0;
            e2 = new_e2; e3 = new_e3; e5 = new_e5;
            cap = new_cap;
        }

        /* store new entry */
        ++size;
        vals[size] = next;
        e2[size] = ex2;
        e3[size] = ex3;
        e5[size] = ex5;

        /* advance all pointers that produced the new value */
        if (v2 == next) ++i2;
        if (v3 == next) ++i3;
        if (v5 == next) ++i5;
    }
    /* ---- result for requested n ---- */
    return (seq_t)(e2[n] + e3[n] + e5[n]);
}
