/*
 * A164614  Expansion of (chi(q) / chi^3(q^3))^2 in powers of q where chi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A164614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   A164614 – Euler transform of the period‑12 sequence * [ 2, -2,
 *            -4, 0, 2, 4, 2, 0, -4, -2, 2, 0 ]. * * Let g(k) = the above
 *            sequence for k ≥ 1 (period 12). * Define s(k) = Σ_{d|k} d·g(d).
 *            * Then a(0)=1 and for n≥1 * n·a(n) = Σ_{k=1}^{n} s(k)·a(n−k). *
 *            * The function below memoises computed values...
 * Range:     i8 n <= 12, i16 n <= 39, i32 n <= 126, i64 n <= 429,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* period‑12 sequence g(k) */
static const int G12[12] =
{ 2, -2, -4, 0, 2, 4, 2, 0, -4, -2, 2, 0 };

/* memoised arrays for a(n) and s(n) */
static seq_t *a_mem = NULL;      /* a(n) */
static seq_t *s_mem = NULL;      /* s(n) */
static size_t mem_cap = 0;       /* current capacity of the arrays */
static idx_t mem_max = 0;        /* largest n already computed */


/* Ensure that arrays are at least capacity n+1 */
static void
ensure_capacity(idx_t n)
{
    if (a_mem == NULL) {
        mem_cap = (size_t)n + 1;
        a_mem = (seq_t *)malloc(mem_cap * sizeof(seq_t));
        s_mem = (seq_t *)malloc(mem_cap * sizeof(seq_t));
        if (a_mem == NULL || s_mem == NULL) { /* allocation failed */
            free(a_mem); free(s_mem);
            a_mem = s_mem = NULL;
            mem_cap = 0;
            return;
        }
        /* initialise */
        a_mem[0] = (seq_t)1;   /* a(0)=1 */
        memset(a_mem + 1, 0, mem_cap - 1);
        memset(s_mem, 0, mem_cap * sizeof(seq_t));
    }

    if (mem_cap <= (size_t)n) {
        /* grow until big enough */
        size_t new_cap = mem_cap ? mem_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp_a = (seq_t *)realloc(a_mem, new_cap * sizeof(seq_t));
        seq_t *tmp_s = (seq_t *)realloc(s_mem, new_cap * sizeof(seq_t));
        if (tmp_a == NULL || tmp_s == NULL) { /* realloc failed */
            return;
        }
        a_mem = tmp_a; s_mem = tmp_s;
        /* zero the new part */
        memset(a_mem + mem_cap, 0, (new_cap - mem_cap) * sizeof(seq_t));
        memset(s_mem + mem_cap, 0, (new_cap - mem_cap) * sizeof(seq_t));
        mem_cap = new_cap;
    }
}


/* Euler transform recurrence */
seq_t
a(idx_t n)
{
    ensure_capacity(n);
    if (a_mem == NULL) return (seq_t)0; /* allocation failed */

    /* extend memoisation up to n */
    for (idx_t m = mem_max + 1; m <= n; ++m) {
        /* compute s(m) = Σ_{d|m} d·g(d) */
        seq_t sm = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                int gd = G12[(d - 1) % 12];
                sm += (seq_t)d * gd;

                idx_t d2 = m / d;
                if (d2 != d) {
                    int gd2 = G12[(d2 - 1) % 12];
                    sm += (seq_t)d2 * gd2;
                }
            }
        }
        s_mem[m] = sm;

        /* m·a(m) = Σ_{k=1}^{m} s(k)·a(m−k) */
        __int128 sum128 = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum128 += (__int128)s_mem[k] * (__int128)a_mem[m - k];
        }
        a_mem[m] = (seq_t)(sum128 / (seq_t)m);
    }
    mem_max = n;
    return a_mem[n];
}
