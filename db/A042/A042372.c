/*
 * A042372  Numerators of continued fraction convergents to sqrt(713).
 * https://oeis.org/A042372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 18, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(0) = 26, a(1) = 27,
 *   a(n) = coeff[n] * a(n-1) + a(n-2)   (n ≥ 2)
 * where coeff[0] = ⌊√713⌋ = 26 and the rest are the periodic
 * continued‑fraction coefficients of √713. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* ----------  continued‑fraction coefficients of √713  ---------- */
static seq_t *coeff = NULL;      /* array of coefficients          */
static size_t coeff_len = 0;     /* number of stored coefficients */
static size_t coeff_cap = 0;     /* capacity of the coeff array   */
static size_t period = 0;        /* number of coefficients after a0 */
static uint64_t a0 = 0;          /* integer part of √713          */

/* Push a coefficient into the coeff array, growing it when needed. */
static void coeff_push(seq_t v)
{
    if (coeff_len == coeff_cap) {
        size_t new_cap = coeff_cap ? (coeff_cap << 1) : 16;
        seq_t *tmp = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
        coeff = tmp;
        coeff_cap = new_cap;
    }
    coeff[coeff_len++] = v;
}

/* Initialise the periodic continued‑fraction coefficients of √713. */
static void init_coeff(void)
{
    if (coeff_len != 0) return;            /* already initialised */

    const uint64_t D = 713ULL;
    a0 = (uint64_t)floor_sqrt(D);          /* integer part of √D */
    coeff_push((seq_t)a0);                  /* first coefficient a0 */

    uint64_t m = 0;
    uint64_t d = 1;
    uint64_t a = a0;

    /* Generate the periodic part until a == 2*a0. */
    while (1) {
        uint64_t m1 = d * a - m;
        uint64_t d1 = (D - m1 * m1) / d;
        uint64_t a1 = (a0 + m1) / d1;
        m = m1;
        d = d1;
        a = a1;
        coeff_push((seq_t)a1);
        if (a1 == 2 * a0) break;
    }
    period = coeff_len - 1;                 /* exclude the first coefficient a0 */
}

/* Return the k‑th continued‑fraction coefficient (k ≥ 0). */
static seq_t coeff_at(idx_t k)
{
    if ((size_t)k < coeff_len) return coeff[(size_t)k];
    /* Repeat the periodic part starting with coefficient a1. */
    idx_t idx = 1 + ((k - 1) % (idx_t)period);
    return coeff[(size_t)idx];
}

/* ----------  numerator array for convergents  ---------- */
static seq_t *num = NULL;      /* array of numerators          */
static size_t num_len = 0;     /* number of computed numerators*/
static size_t num_cap = 0;     /* capacity of the num array    */

/* Ensure the numerator array has capacity for at least 'needed' entries. */
static void ensure_num_cap(size_t needed)
{
    if (needed <= num_cap) return;
    size_t new_cap = num_cap ? (num_cap << 1) : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(num, new_cap * sizeof(seq_t));
    num = tmp;
    num_cap = new_cap;
}

/* Compute the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    init_coeff();                           /* compute CF coefficients once */

    /* Ensure we have space for at least n+1 numerators. */
    ensure_num_cap((size_t)n + 1);

    /* Compute any missing numerators up to index n. */
    for (size_t k = num_len; k <= (size_t)n; ++k) {
        if (k == 0) {
            num[0] = (seq_t)a0;
        } else if (k == 1) {
            /* a(1) = coeff[1] * a(0) + 1 (since p_{-1}=1) */
            seq_t coeff_k = coeff_at(1);
            num[1] = coeff_k * num[0] + (seq_t)1;
        } else {
            seq_t coeff_k = coeff_at((idx_t)k);
            num[k] = coeff_k * num[k - 1] + num[k - 2];
        }
    }

    /* Update the known length if we have computed further. */
    if ((size_t)n + 1 > num_len) num_len = (size_t)n + 1;

    return num[(size_t)n];
}
