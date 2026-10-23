/*
 * A194759  Number of k such that {k*log(2)} > {n*log(2)}, where { } =
 *          fractional part.
 * https://oeis.org/A194759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k, 1 ≤ k ≤ n, such that {k*log(2)} >
 *            {n*log(2)}, where {x} denotes the fractional part of x
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint64_t LOG2_NUM = 12789489753008720204ULL;   /* floor(ln(2) * 2^64) */

seq_t a(idx_t n)
{
    static idx_t current_max = 0;
    static uint64_t *fract = NULL;   /* scaled fractional parts, low 64 bits */
    static seq_t  *result = NULL;    /* a(n) values                         */

    if (n == 0) return 0;            /* for safety, though n≥1 in the API */

    /* extend pre‑computed tables if necessary */
    if (n > current_max) {
        size_t new_size = (size_t)n;
        fract  = (uint64_t *)realloc(fract,  new_size * sizeof(uint64_t));
        result = (seq_t  *)realloc(result, new_size * sizeof(seq_t));

        for (idx_t i = current_max + 1; i <= n; ++i) {
            /* compute scaled fractional part of i*log(2) */
            unsigned __int128 prod  = (unsigned __int128)i * LOG2_NUM;
            uint64_t            fv   = (uint64_t)prod;   /* low 64 bits = fractional part * 2^64 */

            /* count how many previous fractions are larger */
            size_t cnt = 0;
            for (idx_t j = 1; j < i; ++j)
                if (fract[j-1] > fv) ++cnt;

            fract[i-1] = fv;
            result[i-1] = (seq_t)cnt;
        }
        current_max = n;
    }
    return result[n-1];
}
