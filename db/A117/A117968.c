/*
 * A117968  Negative part of inverse of A117966; write -n in balanced ternary
 *          and then replace (-1)'s with 2's.
 * https://oeis.org/A117968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) follows the recurrence a(1) = 2 a(3n) = 3·a(n) a(3n+1)=
 *            3·a(n)+2 a(3n-1)= 3·a(n)+1 This is equivalent to the negative
 *            balanced‑ternary representation described in A117968
 * Range:     u8 n <= 121, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a_rec(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    if (n % 3 == 0)
        return (seq_t)3 * a_rec(n / 3);
    if (n % 3 == 1)
        return (seq_t)3 * a_rec((n - 1) / 3) + (seq_t)2;
    /* n % 3 == 2   i.e. n = 3k‑1 */
    return (seq_t)3 * a_rec((n + 1) / 3) + (seq_t)1;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
