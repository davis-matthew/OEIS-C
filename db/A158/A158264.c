/*
 * A158264  Table where row n lists the coefficients in the (2^n)-th iteration
 *          of x+x^2 for n>=0, read by antidiagonals not including trailing
 *          zeros in rows.
 * https://oeis.org/A158264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term obtained by listing the coefficients of
 *            the polynomials f_{2^k}(x) = g^{2^k}(x) (g(x)=x+x^2) along
 *            antidiagonals of the infinite table, skipping trailing zeros in
 *            rows
 * Range:     u8 n <= 18, u16 n <= 36, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 51 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Excerpt from OEIS A158264 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* allocate Truncated composition P(P(x)) up to degree 'deg' */
static seq_t *compose_trunc(const seq_t *P, size_t Psize, size_t deg)
{
    size_t max_exp = (Psize-1 < deg) ? Psize-1 : deg;
    size_t outSize = deg + 1;
    seq_t *res = mem_new(seq_t, outSize);
    if (!res) return NULL; /* should not happen */

    /* Power 1 = P */
    seq_t **powers = (seq_t **)malloc(max_exp * sizeof(seq_t *));
    if (!powers) { mem_free(res); return NULL; }
    for (size_t i = 0; i < max_exp; ++i) powers[i] = NULL;

    powers[0] = mem_new(seq_t, outSize);
    for (size_t i = 0; i < Psize && i < outSize; ++i)
        powers[0][i] = P[i];

    /* compute higher powers 2..max_exp */
    for (size_t e = 2; e <= max_exp; ++e) {
        const seq_t *prevPow = powers[e-2];
        seq_t *next = mem_new(seq_t, outSize);
        for (size_t a = 0; a <= deg; ++a)
            if (prevPow[a]) {
                for (size_t b = 0; b < Psize && a + b <= deg; ++b) {
                    if (!P[b]) continue;
                    next[a + b] += prevPow[a] * P[b];
                }
            }
        powers[e-1] = next;
    }

    /* accumulate result: sum_{e>=1} P[e] * P^e */
    for (size_t e = 1; e <= max_exp; ++e) {
        seq_t coeff = P[e];
        if (!coeff) continue;
        const seq_t *powVec = powers[e-1];
        for (size_t k = 0; k <= deg; ++k) if (powVec[k]) {
            res[k] += coeff * powVec[k];
        }
    }

    /* cleanup */
    for (size_t i = 0; i < max_exp; ++i)
        mem_free(powers[i]);
    free(powers);

    return res;
}

/* row representation: coeff[e] is coefficient of x^e (coeff[0] = 0) */
typedef struct {
    size_t len;   /* actual length of coeff array */
    seq_t *coeff; /* pointer to coefficient array */
} Row;

#define MAX_ROWS 200          /* enough for n up to about 10^5 */
static Row rows[MAX_ROWS] = { {0, NULL} };

/* compute 2^(2^r) safely for small r (r<=5) */
static inline uint64_t row_length(uint64_t r)
{
    if (r <= 5) {
        return (uint64_t)1 << ((uint64_t)1 << r);
    } else {
        return UINT64_MAX;          /* treated as infinite */
    }
}

/* Ensure that rows[r] has coefficients up to degree 'deg' (require size deg+1). */
static void ensure_row(size_t r, size_t deg)
{
    if (r >= MAX_ROWS) return;      /* safety */

    if (r == 0) {
        /* base polynomial g(x)=x+x^2 : coeff[0]=0, coeff[1]=1, coeff[2]=1 */
        size_t need = deg + 1;
        if (rows[0].coeff) mem_free(rows[0].coeff);
        seq_t *data = mem_new(seq_t, need);
        data[0] = 0;
        if (need > 1) data[1] = 1;
        if (need > 2) data[2] = 1;
        rows[0].coeff = data;
        rows[0].len = need;
        return;
    }

    /* ensure previous row first */
    ensure_row(r-1, deg);

    /* compose previous row with itself, truncated to degree 'deg' */
    seq_t *prev = rows[r-1].coeff;
    size_t prevSize = rows[r-1].len;
    seq_t *newData = compose_trunc(prev, prevSize, deg);
    if (!newData) return; /* should not happen */

    /* free old data if any */
    if (rows[r].coeff) mem_free(rows[r].coeff);
    rows[r].coeff = newData;
    rows[r].len = deg + 1;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    /* find (row, col) corresponding to index n */
    idx_t count = 0;
    for (idx_t s = 0; ; ++s) {
        /* diagonal with sum s, iterate rows from r = s down to 0 */
        for (long long r = (long long)s; r >= 0; --r) {
            uint64_t len = row_length((uint64_t)r);
            idx_t col = s - r;                 /* column index (0-based) */
            if ((uint64_t)col >= len) continue;/* skip trailing zeros */

            if (count == n) {
                size_t neededDeg = (size_t)(col + 1); /* exponent we need (since coeff[0] is constant) */
                if (r >= MAX_ROWS) return 0;         /* safety */
                ensure_row((size_t)r, neededDeg);
                return rows[r].coeff[neededDeg];
            }
            ++count;
        }
    }
    /* unreachable */
    return 0;
}
