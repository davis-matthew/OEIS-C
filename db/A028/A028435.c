/*
 * A028435  Golc sequence in base 5. Left to right concatenation of
 *          n,int(log_5(n)),int(log_5(int(log_5(n)))),... in base5.
 * https://oeis.org/A028435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate the base‑5 representation of n and successive
 *            floor(log_5) values until a 1 is reached
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   append_base5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/append_base5.h"
static inline idx_t floor_log5(idx_t x)
{
    idx_t k = 0;
    while (x >= (idx_t)5) {
        x /= (idx_t)5;
        ++k;
    }
    return k;
}

seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;
    /* first part: n itself in base 5 */
    append_base5(n, &res);

    /* second part: floor(log_5(n)) */
    idx_t log = floor_log5(n);
    append_base5(log, &res);

    /* further parts: successive floor(log_5) values until a 1 is reached */
    while (log > (idx_t)1) {
        log = floor_log5(log);
        append_base5(log, &res);
    }
    return res;
}
