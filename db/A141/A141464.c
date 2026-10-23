/*
 * A141464  Conjectured to be complete list of numbers not of the form 4*x^2 +
 *          3*triangular number + triangular number.
 * https://oeis.org/A141464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A141464: numbers NOT of the form 4*x^2 + 3*T(y)+T(z) with x,y,z>=0   */
/* The function builds a sieve of representable values up to a dynamic limit
   and returns the nth non‑representable integer (1‑based).          */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t cur_limit = 0;          /* current sieve limit                 */
static uint8_t *rep = NULL;          /* representable bitmap                */
static seq_t *terms = NULL;          /* array of non‑representable values   */
static size_t terms_size = 0;        /* number of stored terms              */

static void rebuild(idx_t N) {
    if (rep) free(rep);
    rep = (uint8_t*)calloc((size_t)N + 1, 1);   /* 0‑initialised                 */

    /* mark all representable numbers up to N */
    for (uint64_t x = 0; 4 * x * x <= N; ++x) {
        uint64_t base_x = 4 * x * x;
        for (uint64_t y = 0; ; ++y) {
            uint64_t T_y   = y * (y + 1) / 2;                 /* T(y)          */
            uint64_t base  = base_x + 3 * T_y;
            if (base > N) break;
            for (uint64_t z = 0; ; ++z) {
                uint64_t T_z = z * (z + 1) / 2;             /* T(z)          */
                uint64_t val = base + T_z;
                if (val > N) break;
                rep[val] = 1;
            }
        }
    }

    /* collect non‑representable values into terms array   */
    if (terms) free(terms);
    terms = (seq_t*)malloc(sizeof(seq_t) * ((size_t)N + 1));
    size_t pos = 0;
    for (idx_t v = 1; v <= N; ++v) {
        if (!rep[v]) {
            terms[pos++] = (seq_t)v;
        }
    }
    terms_size = pos;
    cur_limit = N;
}

static void ensure_terms(idx_t n) {
    if (terms_size >= n) return;
    idx_t N = cur_limit;
    if (N < 1024) N = 1024;
    while (true) {
        rebuild(N);
        if (terms_size >= n) return;
        N <<= 1;                 /* double the limit */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* undefined, but safe          */
    ensure_terms(n);
    return terms[n - 1];
}
