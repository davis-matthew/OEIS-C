/*
 * A173343  a(n+4) = a(n+3) - 2*a(n+2) - a(n+1) - a(n).
 * https://oeis.org/A173343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 21, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+4) = a(n+3) - 2*a(n+2) - a(n+1) - a(n), 
   initial values: a(0)=1, a(1)=2, a(2)=0, a(3)=-5.
   Closed form: if n%3==2 then a(n)=0, otherwise
   a(n) = (-1)^(floor(n/3)) * Fib(n+2)  (Fib(0)=0, Fib(1)=1). */
seq_t a(idx_t n)
{
    /* zero occurs at indices 2 mod 3 */
    if ((n % 3) == 2) {
        return 0;
    }

    /* sign depends on floor(n/3) */
    idx_t k = n / 3;

    /* compute Fibonacci(n+2) iteratively */
    idx_t target = n + 2;   /* target index in Fibonacci sequence */
    seq_t f0 = 0;           /* F(0) */
    seq_t f1 = 1;           /* F(1) */
    if (target >= 2) {
        seq_t tmp;
        for (idx_t i = 2; i <= target; ++i) {
            tmp = f0 + f1;
            f0   = f1;
            f1   = tmp;
        }
    }
    /* f1 now holds Fib(target) = Fib(n+2) */
    seq_t fib_val = f1;

    /* apply the sign */
    if (k & 1) {
        return -fib_val;
    } else {
        return fib_val;
    }
}
