/*
 * A086300  Arithmetic derivative of 7-smooth numbers.
 * https://oeis.org/A086300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003415(A002473(n)) (arithmetic derivative of the n‑th
 *            7‑smooth number)
 * Range:     u8 n <= 43, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Exponent vector for the primes 2,3,5,7. */
typedef struct {
    unsigned int e2, e3, e5, e7;
} exp_t;

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Offset is 1 – for n < 1 return 0 (undefined). */
    if (n < 1) return 0;

    /* Static cache of 7‑smooth numbers and their exponent vectors. */
    static idx_t len = 0;          /* number of cached values       */
    static idx_t cap = 0;          /* allocated capacity            */
    static seq_t *nums = NULL;     /* array of 7‑smooth numbers     */
    static exp_t *exps = NULL;     /* corresponding exponent vectors*/
    static idx_t p2 = 0, p3 = 0, p5 = 0, p7 = 0;  /* advance pointers */

    /* Allocate or expand storage if necessary. */
    if (n > cap) {
        idx_t new_cap = cap ? cap << 1 : 32;
        while (new_cap < n) new_cap <<= 1;
        nums = (seq_t *)realloc(nums, new_cap * sizeof(seq_t));
        exps = (exp_t *)realloc(exps, new_cap * sizeof(exp_t));
        cap = new_cap;
    }

    /* Initialise on first call. */
    if (len == 0) {
        len = 1;                     /* nums[0] = 1  is the first 7‑smooth number */
        nums[0] = 1;
        exps[0] = (exp_t){0, 0, 0, 0};
        p2 = p3 = p5 = p7 = 0;
    }

    /* Generate 7‑smooth numbers up to index n. */
    while (len < n) {
        seq_t v2 = nums[p2] * 2;
        seq_t v3 = nums[p3] * 3;
        seq_t v5 = nums[p5] * 5;
        seq_t v7 = nums[p7] * 7;

        /* Determine the next number. */
        seq_t next = v2;
        if (v3 < next) next = v3;
        if (v5 < next) next = v5;
        if (v7 < next) next = v7;

        /* Choose one parent exponent vector and the prime to increment.
           In case of ties, pick the first prime that reaches 'next'. */
        exp_t base;
        int inc2 = 0, inc3 = 0, inc5 = 0, inc7 = 0;
        if (next == v2) { base = exps[p2]; inc2 = 1; }
        else if (next == v3) { base = exps[p3]; inc3 = 1; }
        else if (next == v5) { base = exps[p5]; inc5 = 1; }
        else { base = exps[p7]; inc7 = 1; }

        exp_t cur = base;
        if (inc2) cur.e2++;
        if (inc3) cur.e3++;
        if (inc5) cur.e5++;
        if (inc7) cur.e7++;

        nums[len] = next;
        exps[len] = cur;
        len++;

        /* Advance all pointers that contributed the number. */
        if (next == v2) ++p2;
        if (next == v3) ++p3;
        if (next == v5) ++p5;
        if (next == v7) ++p7;
    }

    /* Arithmetic derivative: d = Σ (e_p * n / p).  Compute with division first
       to avoid overflow when seq_t is a fixed size integer. */
    seq_t x   = nums[n - 1];
    exp_t e   = exps[n - 1];
    seq_t d   = 0;
    if (e.e2) d += (x / 2) * e.e2;
    if (e.e3) d += (x / 3) * e.e3;
    if (e.e5) d += (x / 5) * e.e5;
    if (e.e7) d += (x / 7) * e.e7;

    return d;
}
