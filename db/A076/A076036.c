/*
 * A076036  G.f.: 1/(1 - 5*x*C(x)), where C(x) = (1 - sqrt(1 - 4*x))/(2*x) =
 *          g.f. for the Catalan numbers A000108.
 * https://oeis.org/A076036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (25*a(n-1) - 5*C(n-1)) / 4 for n >= 1 with a(0) = 1 and
 *            Catalan numbers C(0) = 1, C(n) defined by C(n) = C(n-1) * (4*n -
 *            2) / (n + 1). All arithmetic is exact in seq_t
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *A_cache = NULL;   /* a(0 .. n) */
static seq_t *C_cache = NULL;   /* Catalan(0 .. n) */
static size_t  A_cap   = 0;      /* current allocation size */
static size_t  A_size  = 0;      /* one past the largest computed index */

static void ensure_capacity(size_t need)
{
    if (A_cap > need) return;
    size_t newcap = A_cap ? A_cap << 1 : 4;
    while (newcap <= need) newcap <<= 1;

    seq_t *tmp = (seq_t *)realloc(A_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();
    A_cache = tmp;

    tmp = (seq_t *)realloc(C_cache, newcap * sizeof(seq_t));
    if (!tmp) abort();
    C_cache = tmp;

    A_cap = newcap;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (A_size <= idx) {
        ensure_capacity(idx);
        for (size_t i = A_size; i <= idx; ++i) {
            if (i == 0) {
                C_cache[0] = (seq_t)1;   /* Catalan(0) = 1 */
                A_cache[0] = (seq_t)1;   /* a(0)       = 1 */
            } else {
                /* C(i) = C(i-1) * (4*i - 2) / (i + 1) */
                seq_t num = C_cache[i-1] * (seq_t)(4*i - 2);
                C_cache[i] = num / (seq_t)(i + 1);

                /* a(i) = (25*a(i-1) - 5*C(i-1)) / 4 */
                seq_t t = (seq_t)25 * A_cache[i-1];
                t -= (seq_t)5 * C_cache[i-1];
                A_cache[i] = t / (seq_t)4;
            }
        }
        A_size = idx + 1;
    }
    return A_cache[idx];
}
