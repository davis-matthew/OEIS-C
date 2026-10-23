/*
 * A033031  Squarefree kernels of 3-smooth numbers.
 * https://oeis.org/A033031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = radical of the n-th 3‑smooth number. The radical of a
 *            3‑smooth number 2^i 3^j is 1 if i=j=0, 2 if i>0 and j=0, 3 if
 *            i=0 and j>0, 6 if i>0 and j>0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Store exponents (i,j) of the 3‑smooth numbers.
   The array is 1‑based. */
typedef struct { unsigned short i; unsigned short j; } exp_t;

static exp_t *exps = NULL;            /* array of exponents */
static size_t exp_len = 0;            /* number of stored elements (>=1 when initialized) */
static size_t i2 = 1, i3 = 1;         /* pointers for generating next*2 and next*3 */

/* Logarithms of 2 and 3 for double‑precision comparison. */
static const double LOG2 = 0.6931471805599453094172321214581765680755001343602552;
static const double LOG3 = 1.09861228866810969139524523692252570404668841181339;

/* Extend exponents array to at least target elements. */
static void extend_exps(idx_t target)
{
    if (exp_len >= target)
        return;

    /* Ensure target fits into size_t. */
    if (target > SIZE_MAX - 1)
        abort();

    size_t new_len = (size_t)target + 1;
    exp_t *tmp = (exp_t *)realloc(exps, new_len * sizeof(exp_t));
    if (!tmp && new_len > 0)
        abort();   /* allocation failure */

    exps = tmp;

    if (exp_len == 0) {           /* first time initialization */
        exps[1].i = 0;
        exps[1].j = 0;
        i2 = 1;
        i3 = 1;
        exp_len = 1;
    }

    for (idx_t k = (idx_t)(exp_len + 1); k <= target; ++k) {
        exp_t a = exps[i2];    /* value represented by smooth[i2] = 2^a.i * 3^a.j */
        exp_t b = exps[i3];    /* value represented by smooth[i3] = 2^b.i * 3^b.j */

        /* Compute logs of the two candidates: 2*smooth[i2] and 3*smooth[i3] */
        double val2 = (double)(a.i + 1) * LOG2 + (double)a.j * LOG3;
        double val3 = (double)b.i * LOG2 + (double)(b.j + 1) * LOG3;

        if (val2 < val3) {
            /* 2*smooth[i2] is strictly smaller */
            exps[k].i = (unsigned short)(a.i + 1);
            exps[k].j = a.j;
            ++i2;
        } else if (val3 < val2) {
            /* 3*smooth[i3] is strictly smaller */
            exps[k].i = b.i;
            exps[k].j = (unsigned short)(b.j + 1);
            ++i3;
        } else {
            /* Numerical tie – treat as equal and advance both indices. */
            exps[k].i = (unsigned short)(a.i + 1);
            exps[k].j = a.j;
            ++i2;
            ++i3;
        }
    }
    exp_len = (size_t)target;
}

/* Public function: compute a(n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    extend_exps(n);

    const exp_t *e = &exps[n];
    if (e->i == 0 && e->j == 0)
        return (seq_t)1;
    if (e->i > 0 && e->j > 0)
        return (seq_t)6;
    if (e->i > 0)
        return (seq_t)2;
    return (seq_t)3;
}
