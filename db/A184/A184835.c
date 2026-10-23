/*
 * A184835  a(n) = n + floor(n/t) + floor(n/t^2) + floor(n/t^3) +
 *          floor(n/t^4), where t is the pentanacci constant.
 * https://oeis.org/A184835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 none, u64 n <= 117, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n + floor(n/t) + floor(n/t^2) + floor(n/t^3) + floor(n/t^4)
 * where t is the pentanacci constant.
 *
 * The function evaluates each term using integer arithmetic only.
 * It rescales the irrational t and its powers to an integer grid
 * with a precision of 1e12, which is sufficient to get the exact
 * floor values for all n ≤ 10000.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* scaling factor: 10^12 */
    static const unsigned long long SCALE = 1000000000000ULL;

    /* scaled values of t, t^2, t^3, t^4 (each multiplied by SCALE)  */
    static const unsigned long long T  = 1965948236645ULL;   /* t        */
    static const unsigned long long Q  = 3864952469169ULL;   /* t^2      */
    static const unsigned long long R  = 7598296491482ULL;   /* t^3      */
    static const unsigned long long S  = 14937857588930ULL;  /* t^4      */

    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    /* compute n * SCALE using 128‑bit intermediate to avoid overflow */
    seq_t m = (seq_t)((__int128)n_seq * SCALE);

    seq_t f1 = m / (seq_t)T;         /* floor(n/t)       */
    seq_t f2 = m / (seq_t)Q;         /* floor(n/t^2)     */
    seq_t f3 = m / (seq_t)R;         /* floor(n/t^3)     */
    seq_t f4 = m / (seq_t)S;         /* floor(n/t^4)     */

    return n_seq + f1 + f2 + f3 + f4;
}
