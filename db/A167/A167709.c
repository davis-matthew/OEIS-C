/*
 * A167709  Numbers y such that 19*y^2 + 81 is a square.
 * https://oeis.org/A167709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A167709: numbers y such that 19*y^2 + 81 is a square.
 * Recurrence used (valid for all n >= 0):
 *   a(n + 10) = 340 * a(n + 5) - a(n)
 * Initial values for the five residue classes modulo 5 are:
 *   r = 0 : a(0)  = 0,  a(5)  = 351
 *   r = 1 : a(1)  = 1,  a(6)  = 560
 *   r = 2 : a(2)  = 15, a(7)  = 5124
 *   r = 3 : a(3)  = 24, a(8)  = 8175
 *   r = 4 : a(4)  = 220, a(9) = 74801
 *
 * The function below memoises each residue class separately so that
 * arbitrary calls (in any order) to a(idx_t n) are answered in O(1)
 * amortised time after a linear precomputation within that residue.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* residue class remainder modulo 5 */
    idx_t r = n % 5;
    /* index within the subsequence (r, r+5, r+10, ...) */
    idx_t m = n / 5;

    /* initial two terms for each residue class */
    static const seq_t init[5][2] = {
        { (seq_t)0,      (seq_t)351 },
        { (seq_t)1,      (seq_t)560 },
        { (seq_t)15,     (seq_t)5124 },
        { (seq_t)24,     (seq_t)8175 },
        { (seq_t)220,    (seq_t)74801 }
    };

    /* If the requested term is one of the first two, return immediately */
    if (m < 2)
        return init[r][m];

    /* Per‑residue memoisation structures */
    static seq_t *seq[5]      = { NULL, NULL, NULL, NULL, NULL };
    static size_t n_computed[5] = { 0, 0, 0, 0, 0 }; /* number of terms stored */
    static size_t cap[5]        = { 0, 0, 0, 0, 0 }; /* allocated capacity */

    /* Initialise storage for this residue if required */
    if (n_computed[r] == 0) {
        /* allocate space for the first two known values */
        size_t init_cap = 4;  /* start with a small capacity */
        seq[r] = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!seq[r]) exit(1);
        seq[r][0] = init[r][0];
        seq[r][1] = init[r][1];
        n_computed[r] = 2;
        cap[r] = init_cap;
    }

    /* Extend the memoised values up to index m (if not already computed) */
    while (n_computed[r] <= (size_t)m) {
        /* Ensure enough space */
        if (n_computed[r] >= cap[r]) {
            size_t new_cap = cap[r] * 2;
            if (new_cap == 0) new_cap = 4;
            seq_t *tmp = (seq_t *)realloc(seq[r], new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            seq[r] = tmp;
            cap[r] = new_cap;
        }

        /* Compute next term using the recurrence a(n+10)=340*a(n+5)-a(n) */
        seq_t last   = seq[r][n_computed[r] - 1];
        seq_t prev   = seq[r][n_computed[r] - 2];
        seq_t next   = 340 * last - prev;   /* operations performed in seq_t type */

        seq[r][n_computed[r]] = next;
        ++n_computed[r];
    }

    return seq[r][m];
}
