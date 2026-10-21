/*
 * A221073  Simple continued fraction expansion of an infinite product.
 * https://oeis.org/A221073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, cofr
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 48, u64 n <= 98, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=2; a(2k)=1 (k≥1).
 * For odd indices use two independent recurrences:
 *   let n≡1 (mod 4):  n = 4m-3, m≥1
 *       p_1 = 4,   p_2 = 32,
 *       p_{i+1} = 6·p_i − p_{i-1} + 8
 *       a(n) = p_m
 *
 *   let n≡3 (mod 4):  n = 4m-1, m≥1
 *       q_1 = 8,   q_2 = 56,
 *       q_{i+1} = 6·q_i − q_{i-1} + 8
 *       a(n) = q_m
 *
 * These linear recurrences come from the closed‑form expressions
 * involving (√2±1)^k.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    /* even indices (except 0) are 1 */
    if ((n & 1) == 0)
        return (seq_t)1;

    /* odd indices */
    if ((n & 3) == 1) {           /* n = 4m-3 */
        idx_t m = (n + 3) / 4;     /* m ≥ 1 */
        if (m == 1)
            return (seq_t)4;
        if (m == 2)
            return (seq_t)32;
        seq_t p_prev_prev = (seq_t)4;   /* p1 */
        seq_t p_prev = (seq_t)32;       /* p2 */
        seq_t p_curr = 0;
        for (idx_t i = 3; i <= m; ++i) {
            p_curr = ((seq_t)6) * p_prev - p_prev_prev + ((seq_t)8);
            p_prev_prev = p_prev;
            p_prev = p_curr;
        }
        return p_curr;
    } else {                       /* n = 4m-1 */
        idx_t m = (n + 1) / 4;     /* m ≥ 1 */
        if (m == 1)
            return (seq_t)8;
        if (m == 2)
            return (seq_t)56;
        seq_t q_prev_prev = (seq_t)8;   /* q1 */
        seq_t q_prev = (seq_t)56;       /* q2 */
        seq_t q_curr = 0;
        for (idx_t i = 3; i <= m; ++i) {
            q_curr = ((seq_t)6) * q_prev - q_prev_prev + ((seq_t)8);
            q_prev_prev = q_prev;
            q_prev = q_curr;
        }
        return q_curr;
    }
}
