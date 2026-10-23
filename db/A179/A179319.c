/*
 * A179319  G.f.: WL(-x)*WU(x), where WL, WU are respectively the
 *          characteristic functions of the lower (A000201) and upper
 *          (A001950) Wythoff sequences.
 * https://oeis.org/A179319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{i=0}^{n} (-1)^i · WL(i) · WU(n-i) where WL(i)=1 iff
 *            i∈A000201 (lower Wythoff numbers), WU(j)=1 iff j∈A001950 (upper
 *            Wythoff numbers). Since 0 belongs to both sequences, WU(j) =
 *            (j==0) || !WL(j)
 * Range:     i8 n <= 76, i16 n <= 76, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint8_t *lower_mem = NULL;   /* 1 if index in A000201, 0 otherwise */
static size_t lower_cap   = 0;      /* allocated size in elements            */
static idx_t next_k       = 0;      /* next k for generating floor(k*phi)   */
static const long double phi = 1.6180339887498948482045868343656381177203L;

static void ensure_lower_mem(idx_t n)
{
    if (lower_cap > (size_t)n)
        return;
    size_t new_cap = (size_t)(n + 1);
    uint8_t *tmp = (uint8_t *)realloc(lower_mem, new_cap);
    if (!tmp)
        abort();
    memset(tmp + lower_cap, 0, new_cap - lower_cap);
    lower_mem = tmp;
    lower_cap = new_cap;

    while (1) {
        long double d = (long double)next_k * phi;
        idx_t a = (idx_t)d;
        if (a > n)
            break;
        lower_mem[a] = 1;
        ++next_k;
    }
}

seq_t a(idx_t n)
{
    ensure_lower_mem(n);
    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i <= n; ++i) {
        if (!lower_mem[i])
            continue;
        idx_t j = n - i;
        if (j == 0 || !lower_mem[j]) {
            if (i & 1)
                sum -= (seq_t)1;
            else
                sum += (seq_t)1;
        }
    }
    return sum;
}
