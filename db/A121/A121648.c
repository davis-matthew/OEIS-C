/*
 * A121648  a(n) is the square of the coefficient of x^n in 1/(1 - x*A(x^2)),
 *          where g.f. A(x) = Sum_{n>=0} a(n)*x^n.
 * https://oeis.org/A121648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A121648:  a(n) = b(n)^2 where b(0)=1
 * and for n≥1  b(n) = Σ_{i=0}^{⌊(n‑1)/2⌋} b(i)^2 * b(n‑2i‑1)   (OEIS A121649)
 */

#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

static seq_t *bArr = NULL;          /* b(n) = sqrt(a(n))   */
static seq_t *sqArr = NULL;         /* sqArr[n] = b(n)^2    */
static size_t cap = 0;              /* current allocation   */
static size_t computed = (size_t)-1;/* highest index computed; (size_t)-1 means none */

/* Ensure space for indices 0..n */
static void ensure_capacity(size_t n)
{
    if (cap > n) return;
    size_t newcap = cap ? cap << 1 : 1;
    while (newcap <= n) newcap <<= 1;
    seq_t *newb  = (seq_t *)realloc(bArr,  newcap * sizeof(seq_t));
    seq_t *newsq = (seq_t *)realloc(sqArr, newcap * sizeof(seq_t));
    assert(newb && newsq);          /* abort on allocation failure */
    bArr  = newb;
    sqArr = newsq;
    cap   = newcap;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    ensure_capacity(N);

    /* initialize base case once */
    if (computed == (size_t)-1) {
        bArr[0]  = 1;
        sqArr[0] = 1;
        computed = 0;
    }

    /* compute missing terms up to N */
    for (size_t i = computed + 1; i <= N; ++i) {
        seq_t sum = 0;
        size_t limit = (i - 1) / 2;
        for (size_t m = 0; m <= limit; ++m) {
            size_t idx = i - 2 * m - 1;          /* n‑2i‑1 */
            sum += sqArr[m] * bArr[idx];        /* b(m)^2 * b(idx) */
        }
        bArr[i]  = sum;
        sqArr[i] = sum * sum;
    }
    if (N > computed) computed = N;
    return sqArr[N];
}
