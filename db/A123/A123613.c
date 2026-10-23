/*
 * A123613  Column 3 of triangle A123610.
 * https://oeis.org/A123613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   The sequence is obtained from the generating function P_3(x) /
 *            ((1-x)^2 (1-x^2)^2 (1-x^3)^2) where
 *            P_3(x)=1+2x+11x^2+26x^3+30x^4+26x^5+17x^6+6x^7+x^8. Let
 *            C(m)=∑_{k≤m/3} (k+1)*S(m-3k) with S(p)=(p-2⌊p/2⌋+1)(⌊p/2⌋+1) as
 *            derived in the OEIS entry. Then a(n)=∑_{t=0}^8 P_t·C(n−t). This
 *            implementation...
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 171, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Cache for the intermediate coefficients C(m) */
static seq_t *C = NULL;          /* array of seq_t, capacity C_cap */
static size_t C_cap = 0;         /* allocated capacity                 */
static size_t C_len = 0;         /* current number of computed entries */

/* Grow the cache to cover all indices up to n inclusive.
   The function assumes C_len ≤ n. */
static void ensure_C(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= C_cap)
        return;

    void *tmp = realloc(C, needed * sizeof(seq_t));
    if (!tmp)
        abort(); /* allocation failed */
    C = (seq_t *)tmp;
    C_cap = needed;

    /* Compute new entries C[m] for m = C_len .. n */
    for (idx_t m = C_len; m <= n; ++m) {
        seq_t cum = 0;
        idx_t maxk = m / 3;
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t p = m - 3 * k;           /* p = m - 3k  */
            seq_t s;
            if ((p & 1ULL) == 0) {         /* even p = 2q */
                idx_t q = p / 2;
                seq_t a = (seq_t)(q + 1);
                seq_t b = (seq_t)(q + 2);
                seq_t d = (seq_t)(2 * q + 3);
                seq_t prod = a * b;
                prod *= d;
                s = prod / 6;
            } else {                       /* odd p = 2q+1 */
                idx_t q = (p - 1) / 2;
                seq_t a = (seq_t)(q + 1);
                seq_t b = (seq_t)(q + 2);
                seq_t d = (seq_t)(q + 3);
                seq_t prod = a * b;
                prod *= d;
                s = prod / 3;
            }
            seq_t term = (seq_t)(k + 1) * s;
            cum += term;
        }
        C[m] = cum;
    }

    C_len = n + 1;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    ensure_C(n);

    /* Coefficients of P_3(x) */
    static const seq_t P[9] = {
        (seq_t)1,  (seq_t)2,  (seq_t)11,
        (seq_t)26, (seq_t)30, (seq_t)26,
        (seq_t)17, (seq_t)6,  (seq_t)1
    };

    seq_t result = 0;
    for (int t = 0; t < 9 && (idx_t)t <= n; ++t) {
        result += P[t] * C[n - t];
    }
    return result;
}
