/*
 * A033034  Numbers all of whose base 8 digits are odd.
 * https://oeis.org/A033034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   For n≥1, write n‑1 in base‑4, pad to the minimal digit length d
 *            such that Σ_{k=1}^{d} 4^k ≥ n, map each base‑4 digit d to an odd
 *            base‑8 digit 2*d+1, and interpret the result in base 8. This
 *            yields the OEIS sequence A033034 – numbers whose base‑8 digits
 *            are all odd
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, return 0 */

    /* Find the minimal number of base‑8 digits d that cover n. */
    idx_t d      = 1;                    /* current digit length   */
    idx_t count  = 4;                    /* 4^d  (for d=1)         */
    idx_t cum    = 4;                    /* cumulative up to d     */
    while (n > cum) {
        ++d;
        count *= 4;                      /* compute 4^d             */
        cum   += count;                  /* cumulative up to d     */
    }

    idx_t prev_cum = cum - count;        /* cumulative up to d‑1    */
    idx_t m = n - prev_cum - 1;          /* zero‑based offset in block */

    seq_t result = 0;
    seq_t power  = 1;                    /* 8^i for digit position i */

    /* Build the number digit by digit. */
    for (idx_t i = 0; i < d; ++i) {
        uint32_t digit = (uint32_t)(m & 3U);    /* m mod 4 */
        seq_t odd = (seq_t)(2 * digit + 1);
        result += odd * power;
        power  *= (seq_t)8;
        m >>= 2;                            /* m / 4 */
    }
    return result;
}
