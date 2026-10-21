/*
 * A070690  Duplicate of A070471.
 * https://oeis.org/A070690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^3 mod 5
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n mod 5 is at most 4, so all intermediate products fit easily in
       any integer type.  We use idx_t for the modulus operation and cast
       to seq_t only for the final result. */
    idx_t n_mod = n % 5ULL;
    seq_t t = (seq_t)n_mod * (seq_t)n_mod;   /* n^2 mod 5 */
    t = (t * (seq_t)n_mod) % 5;              /* n^3 mod 5 */
    return t;
}
