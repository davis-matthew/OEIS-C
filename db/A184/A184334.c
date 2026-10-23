/*
 * A184334  Period 6 sequence [0, 2, 2, 0, -2, -2, ...] except a(0) = 1.
 * https://oeis.org/A184334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1; for n>=1 the sequence is periodic of length 6: a(n) =
 *            [0, 2, 2, 0, -2, -2]_{n mod 6}. Hence a(1)=2, a(2)=2, a(3)=0,
 *            a(4)=-2, a(5)=-2, a(6)=0, a(7)=2, …
 * Range:     i8 n <= 2500, i16 n <= 2500, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    switch (n % 6) {
        case 0: return (seq_t)0;      /* n ≡ 0 (mod 6) */
        case 1: return (seq_t)2;      /* n ≡ 1 (mod 6) */
        case 2: return (seq_t)2;      /* n ≡ 2 (mod 6) */
        case 3: return (seq_t)0;      /* n ≡ 3 (mod 6) */
        case 4: return (seq_t)-2;     /* n ≡ 4 (mod 6) */
        case 5: return (seq_t)-2;     /* n ≡ 5 (mod 6) */
    }
    /* Should never reach here */
    return (seq_t)0;
}
