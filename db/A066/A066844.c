/*
 * A066844  Film speeds.
 * https://oeis.org/A066844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A066844 – Film speeds
 *
 * Sequence definition:
 *   a(1) = 25
 *   a(2) = 40
 *   For n≥3 let the prime‑factor exponents of a(n‑1) be
 *   a(n‑1) = 2^p * 5^q.
 *   If p ≥ q  then a(n) = a(n‑1) * 5 / 4
 *   else         a(n) = a(n‑1) * 32 / 25
 *   The rule generates the film speed values.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‐of‑range */
    if (n == 1) return (seq_t)25;         /* first term */

    seq_t  val = (seq_t)25;                /* current term            */
    idx_t  p   = 0;                        /* exponent of 2           */
    idx_t  q   = 2;                        /* exponent of 5           */

    /* second term */
    if (n == 2) {
        val = (val / (seq_t)5) * (seq_t)8;
        return val;
    }

    /* apply the special first multiplier 8/5 */
    val = (val / (seq_t)5) * (seq_t)8;     /* now at index 2          */
    p  += 3;  /* 2^3   */
    q  -= 1;  /* 5^-1  */
    idx_t idx = 2;                         /* current index           */

    /* iterate until we reach n */
    while (idx < n) {
        if (p >= q) {                     /* multiply by 5/4         */
            val = (val / (seq_t)4) * (seq_t)5;
            p -= 2;
            q += 1;
        } else {                          /* multiply by 32/25       */
            val = (val / (seq_t)25) * (seq_t)32;
            p += 5;
            q -= 2;
        }
        ++idx;
    }
    return val;
}
