/*
 * A166871  Permutation of the integers: 3 positives, 2 negatives.
 * https://oeis.org/A166871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = the (n‐th) term of a stream that lists three positive
 *            integers followed by two negative integers, repeating with the
 *            natural numbers in order
 * Range:     i8 n <= 211, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n = 0 -> 0;  else for n>=1:
   let q = (n-1)/5, r = (n-1)%5
   if r<3  -> return 3*q + (r+1)
   else    -> return -(2*q + (r-2))                       */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    const idx_t block = 5;          // 3 positives + 2 negatives
    const idx_t pos  = 3;          // p
    const idx_t neg  = 2;          // n

    idx_t q = (n - 1) / block;
    idx_t r = (n - 1) % block;

    if (r < pos) {
        /* Positive term */
        seq_t result = (seq_t)( (seq_t)pos * (seq_t)q + (seq_t)(r + 1) );
        return result;
    } else {
        /* Negative term */
        seq_t result = -( (seq_t)neg * (seq_t)q + (seq_t)(r - pos + 1) );
        return result;
    }
}
