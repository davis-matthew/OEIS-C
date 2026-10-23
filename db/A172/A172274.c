/*
 * A172274  a(n) = floor(n*(sqrt(13)-sqrt(11))).
 * https://oeis.org/A172274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(sqrt(13)-sqrt(11)))
 * Range:     u8 n <= 2, u16 n <= 19, u32 n <= 703, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses only integer arithmetic.
   It relies on the helper floor_sqrt(seq_t x) from
   "../utils/floor_sqrt.h" to obtain floor(√x) for arbitrary-precision
   integers.
   The algorithm is based on the identity
        a(n) = floor( s13 - s11 - δ(n) )
   where s13 = ⌊√(13 n²)⌋, s11 = ⌊√(11 n²)⌋ and δ(n) is 0 or 1.
   The sign of δ(n) is determined by the inequality
        44·c²·n² > (2·n² – c²)²
   with c = s13 – s11.  This inequality is equivalent to
   (√(11) n + c)² > (√(13) n)², i.e.  √(13) n < √(11) n + c,
   which is exactly the condition for a(n) to be c‑1 instead of c. */
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Handle the trivial offset case directly. */
    if (n == 0u)
        return (seq_t)0;

    /* Work entirely in seq_t to avoid overflow for large indices. */
    seq_t n_seq  = (seq_t)n;
    seq_t n2     = n_seq * n_seq;                /* n²                           */
    seq_t t13    = n2 * (seq_t)13;               /* 13 n²                        */
    seq_t t11    = n2 * (seq_t)11;               /* 11 n²                        */

    /* Integer square roots of the scaled numbers. */
    seq_t s13    = floor_sqrt(t13);             /* ⌊√(13 n²)⌋                   */
    seq_t s11    = floor_sqrt(t11);             /* ⌊√(11 n²)⌋                   */

    /* Candidate value (may need to be reduced by one). */
    seq_t candidate = s13 - s11;                /* c = s13 – s11                 */
    seq_t cand2     = candidate * candidate;    /* c²                            */

    /* Inequality testing: 44·c²·n² > (2·n² – c²)²   ⇒ subtract 1 */
    seq_t lhs  = ((seq_t)44) * cand2 * n2;       /* 44·c²·n²                     */
    seq_t temp = (seq_t)2 * n2 - cand2;          /* 2·n² – c²                    */
    seq_t rhs  = temp * temp;                    /* (2·n² – c²)²                 */

    /* Determine whether to subtract 1 from the candidate. */
    seq_t subtract = (lhs > rhs) ? (seq_t)1 : (seq_t)0;

    return candidate - subtract;
}
