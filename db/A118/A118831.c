/*
 * A118831  Numerators of the convergents of the 2-adic continued fraction of
 *          zero given by A118830.
 * https://oeis.org/A118831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(n) = pattern[(n-1) mod 8] where * pattern = [-1, -1, 0, -1, 1,
 *            1, 0, 1]; * Equivalently, a(n) = -a(n-4) for n > 4
 * Range:     i8 n <= 107, i16 n <= 107, i32 n <= 107, i64 n <= 107,
 *            i128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _A118831_pattern[8] = {
    (seq_t)-1,
    (seq_t)-1,
    (seq_t)0,
    (seq_t)-1,
    (seq_t)1,
    (seq_t)1,
    (seq_t)0,
    (seq_t)1
};

seq_t a(idx_t n)
{
    /* (n-1) % 8 can be computed with bitwise AND since 8 is a power of two. */
    idx_t k = (n - (idx_t)1) & (idx_t)7;
    return _A118831_pattern[k];
}
