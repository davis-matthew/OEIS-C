/*
 * A126977  A 3 x 3 multiplicative magic square read by rows.
 * https://oeis.org/A126977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: full, pass, 9 terms through n = 9 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of 3×3 multiplicative magic squares with product 216,
   read row‑wise.  Each square is obtained by solving the exponent
   equations for the primes 2 and 3 (exponents 0‑3), constructing the
   nine entries, discarding squares with repeated entries, sorting
   the remaining squares lexicographically, and then flattening them
   into a one‑dimensional sequence.  The function a(n) returns the
   n‑th element (1‑based). */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Generate all exponent solutions for one prime using the equations
   derived from the magic‑product condition.  Solutions are put into
   the supplied array.  The array must be pre‑allocated large enough
   for all solutions (46 in total). */
static void generate_prime_sols(uint8_t (*sol)[9], size_t *count)
{
    *count = 0;
    for (uint8_t a = 0; a < 4; ++a)
    for (uint8_t b = 0; b < 4; ++b)
    for (uint8_t c = 0; c < 4; ++c)
    for (uint8_t d = 0; d < 4; ++d)
    for (uint8_t e = 0; e < 4; ++e)
    for (uint8_t f = 0; f < 4; ++f)
    for (uint8_t g = 0; g < 4; ++g)
    for (uint8_t h = 0; h < 4; ++h)
    for (uint8_t i = 0; i < 4; ++i)
    {
        if (a + b + c != 3) continue;
        if (d + e + f != 3) continue;
        if (g + h + i != 3) continue;
        if (a + d + g != 3) continue;
        if (b + e + h != 3) continue;
        if (c + f + i != 3) continue;
        if (a + e + i != 3) continue;
        if (c + e + g != 3) continue;
        uint8_t *p = sol[*count];
        p[0] = a; p[1] = b; p[2] = c;
        p[3] = d; p[4] = e; p[5] = f;
        p[6] = g; p[7] = h; p[8] = i;
        ++(*count);
    }
}

/* Pre‑computed powers of 2 and 3 for exponents 0..3 */
static const seq_t pow2[4] = {1, 2, 4, 8};
static const seq_t pow3[4] = {1, 3, 9, 27};

/* Structure holding a square's nine entries */
struct Square {
    seq_t v[9];
};

/* Compare two squares for qsort: lexicographic order of the nine
   entries. */
static int cmp_squares(const void *p1, const void *p2)
{
    const struct Square *s1 = (const struct Square *)p1;
    const struct Square *s2 = (const struct Square *)p2;
    for (size_t k = 0; k < 9; ++k) {
        if (s1->v[k] < s2->v[k]) return -1;
        if (s1->v[k] > s2->v[k]) return 1;
    }
    return 0;
}

/* a(n) – 1‑based index, external linkage. */
seq_t a(idx_t n)
{
    static bool init = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    if (!init) {
        /* ------------------------------------
           Generate all exponent matrices for one prime
          ------------------------------------ */
        const size_t max_candidate = 262144; /* 4^9 */
        uint8_t (*prime_sols)[9] = (uint8_t (*)[9])malloc(max_candidate * sizeof(uint8_t[9]));
        if (!prime_sols) return 0;  /* allocation failed */
        size_t prime_cnt = 0;
        generate_prime_sols(prime_sols, &prime_cnt);  /* 46 solutions */

        /* ------------------------------------
           Generate all distinct squares
          ------------------------------------ */
        struct Square *sqs = NULL;
        size_t sq_cnt = 0, sq_cap = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            const uint8_t *e2 = prime_sols[i];
            for (size_t j = 0; j < prime_cnt; ++j) {
                const uint8_t *e3 = prime_sols[j];
                struct Square s;
                bool distinct = true;
                for (size_t k = 0; k < 9; ++k) {
                    s.v[k] = pow2[e2[k]] * pow3[e3[k]];
                }
                /* check distinctness */
                for (size_t x = 0; x < 9 && distinct; ++x)
                    for (size_t y = x + 1; y < 9; ++y)
                        if (s.v[x] == s.v[y]) { distinct = false; break; }
                if (!distinct) continue;

                if (sq_cnt == sq_cap) {
                    sq_cap = sq_cap ? sq_cap << 1 : 32;
                    sqs = mem_resize(struct Square, sqs, sq_cap);
                    if (!sqs) { free(prime_sols); return 0; }
                }
                sqs[sq_cnt++] = s;
            }
        }
        free(prime_sols);

        /* ------------------------------------
           Sort squares lexicographically and deduplicate
          ------------------------------------ */
        mem_qsort(sqs, sq_cnt, sizeof(struct Square), cmp_squares);

        size_t uniq_cnt = 0;
        struct Square *uniq = NULL;
        size_t uniq_cap = 0;
        for (size_t i = 0; i < sq_cnt; ++i) {
            if (i == 0 || cmp_squares(&sqs[i], &sqs[i - 1]) != 0) {
                if (uniq_cnt == uniq_cap) {
                    uniq_cap = uniq_cap ? uniq_cap << 1 : 32;
                    uniq = mem_resize(struct Square, uniq, uniq_cap);
                    if (!uniq) { mem_free(sqs); return 0; }
                }
                uniq[uniq_cnt++] = sqs[i];
            }
        }
        mem_free(sqs);

        /* ------------------------------------
           Build flattened sequence
          ------------------------------------ */
        seq_len = uniq_cnt * 9;
        seq = mem_new(seq_t, seq_len);
        if (!seq) { mem_free(uniq); return 0; }
        for (size_t i = 0; i < uniq_cnt; ++i)
            mem_copy(seq + i * 9, uniq[i].v, 9 * sizeof(seq_t));

        mem_free(uniq);
        init = true;
    }

    /* safety: if n is out of bounds, return 0 */
    if (n < ((idx_t)1) || (size_t)(n - 1) >= seq_len)
        return 0;
    return seq[(size_t)(n - 1)];
}
