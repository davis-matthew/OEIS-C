/*
 * A186832  Number of (n+2)X3 binary arrays with 3X3 subblock sums
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A186832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{r=0}^{2} C( floor((n+1-r)/3)+1 ), where C(m) is the
 *            coefficient of x^m in 1/((1‑x)^2 (1‑3x)^2). C satisfies C(0)=1,
 *            C(1)=8, C(2)=42, C(3)=184 C(m)=8*C(m‑1) – 22*C(m‑2) + 24*C(m‑3)
 *            – 9*C(m‑4) (m≥4)
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 29, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/*   The types seq_t and idx_t are supplied by the build system   */

/*  Cache for the sequence C(m)  */
static seq_t *C = NULL;      /* array of terms C[0 .. C_len-1]      */
static size_t C_len = 0;     /* number of terms currently stored   */
static size_t C_cap = 0;     /* allocated capacity of C             */

/*  Ensure that terms up to n_max  (inclusive) are available  */
static void ensure_C(size_t n_max)
{
    if (n_max < C_len)
        return;

    /* grow the buffer */
    size_t new_cap = C_cap ? C_cap : 4;
    while (new_cap <= n_max)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(C, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    C = tmp;
    C_cap = new_cap;

    /* initialise first four values if necessary */
    if (C_len == 0) {
        C[0] = (seq_t)1;
        C[1] = (seq_t)8;
        C[2] = (seq_t)42;
        C[3] = (seq_t)184;
        C_len = 4;
    } else if (C_len < 4) {               /* fill missing initials   */
        if (C_len <= 0) { C[0] = (seq_t)1; C_len = 1; }
        if (C_len <= 1) { C[1] = (seq_t)8; C_len = 2; }
        if (C_len <= 2) { C[2] = (seq_t)42; C_len = 3; }
        if (C_len <= 3) { C[3] = (seq_t)184; C_len = 4; }
    }

    /* compute required new terms by recurrence */
    for (size_t i = C_len; i <= n_max; ++i) {
        seq_t val = (seq_t)8 * C[i-1];
        val -= (seq_t)22 * C[i-2];
        val += (seq_t)24 * C[i-3];
        val -= (seq_t)9  * C[i-4];
        C[i] = val;
    }
    C_len = n_max + 1;
}

/*  OEIS sequence A186832  */
seq_t a(idx_t n)
{
    /* compute lengths of the three chains modulo 3 */
    idx_t t = n + 1;                    /* n+1 is used in the formulas */
    size_t m0 = (size_t)(t / 3)       + 1;   /* floor((n+1)/3)+1       */
    size_t m1 = (size_t)((t-1) / 3)   + 1;   /* floor((n  )/3)+1       */
    size_t m2 = (size_t)((t-2) / 3)   + 1;   /* floor((n-1)/3)+1       */

    /* find the maximum needed term of C(m)  */
    size_t max_m = m0;
    if (m1 > max_m) max_m = m1;
    if (m2 > max_m) max_m = m2;

    ensure_C(max_m);

    seq_t result = C[m0];
    result *= C[m1];
    result *= C[m2];
    return result;
}
