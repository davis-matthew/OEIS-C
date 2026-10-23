/*
 * A170373  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A170373: Number of reduced words of length n in a Coxeter group
 * of rank 28 with (S_i)^2 = (S_i S_j)^43 = I.
 *
 * The ordinary generating function is  
 *   G(t)= (t^43 + 2*t^42 + … + 2*t + 1) / (351*t^43 - 26*t^42 – … – 26*t + 1)
 *
 * From this rational form we obtain a linear recurrence of order 43:
 *
 *   a(n) = 26 · (a(n-1)+…+a(n-42)) – 351 · a(n-43)   for n ≥ 43
 *
 * Initial values for 0 ≤ n ≤ 42 are derived from the numerator:
 *
 *   a(0)=1,  a(1)=28,  a(2)=756, …, a(42) computed by
 *   a(n) = 2 + 26·Σ_{k=0}^{n-1} a(k)   (n≥1)
 *
 * This implementation memoises the values so that successive calls are
 * processed in O(1) after an initial O(43) set-up and handles any order of
 * input.
 */

#include <stdbool.h>
#include <stdint.h>

#define ORD 43  /* order of the recurrence */

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t buf[ORD];      /* circular buffer of the last ORD terms */
    static seq_t sum42;         /* sum of the last 42 terms (excluding oldest) */
    static int idx;             /* position of the oldest term in buf */
    static idx_t computed;      /* number of terms that have been computed so far */

    /* ---- initialisation (once) ----------------------------------- */
    if (!init) {
        /* a(0) */
        buf[0] = (seq_t)1;
        seq_t cum = buf[0];                /* Σ_{k=0}^{i-1} a(k) */

        /* a(1) … a(42) */
        for (idx_t i = 1; i < ORD; ++i) {
            seq_t val = (seq_t)2 + (seq_t)26 * cum;  /* a(i) = 2 + 26*Σ_{k=0}^{i-1} a(k) */
            buf[i] = val;
            cum += val;                         /* update cumulative sum */
        }

        /* sum of the last 42 terms (indices 1..42) */
        sum42 = cum - buf[0];

        idx = 0;              /* oldest term is a(0) */
        computed = ORD;       /* we have computed a(0) … a(42) */
        init = true;
    }

    /* ---- return directly for already available terms ------------ */
    if (n < ORD) {
        return buf[n];
    }

    /* ---- compute terms up to n (inclusive) ----------------------- */
    while (computed <= n) {
        seq_t oldest = buf[idx];
        seq_t new_val = (seq_t)26 * sum42 - (seq_t)351 * oldest;
        buf[idx] = new_val;               /* overwrite oldest */
        sum42 = sum42 + new_val - oldest; /* update sum of 42 recent terms */
        idx = (idx + 1) % ORD;            /* advance circular buffer */
        ++computed;
    }

    /* ---- retrieve the requested term -------------------------------- */
    int pos = (idx + ORD - 1) % ORD;       /* buffer position of a(n) */
    return buf[pos];
}
