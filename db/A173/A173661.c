/*
 * A173661  Logarithmic derivative of the squares of the Fibonacci numbers
 *          (A007598, with offset).
 * https://oeis.org/A173661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n)^2 for odd n; a(n) = Lucas(n)^2 - 2 for even n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

struct fib_pair {
    seq_t f;     /* F(n)   */
    seq_t f_next;/* F(n+1) */
};

static inline struct fib_pair fib_pair(idx_t n)
/* Fast‑doubling algorithm to return (F(n),F(n+1)). */
{
    if (n == 0) {
        return (struct fib_pair){ (seq_t)0, (seq_t)1 };
    }
    struct fib_pair p = fib_pair(n >> 1);
    seq_t a = p.f;
    seq_t b = p.f_next;
    seq_t two_b_minus_a = (b + b) - a; /* 2*F(k+1)-F(k)   */
    seq_t c = a * two_b_minus_a;      /* F(2k)            */
    seq_t d = a * a + b * b;          /* F(2k+1)          */
    if ((n & (idx_t)1ULL) == 0) {
        return (struct fib_pair){ c, d };
    } else {
        return (struct fib_pair){ d, c + d };
    }
}

seq_t a(idx_t n)
/* Compute a(n) as defined. */
{
    /* Compute Lucas(n) = F(n-1)+F(n+1). */
    struct fib_pair pn1 = fib_pair(n - 1); /* F(n-1), F(n)   */
    struct fib_pair pn  = fib_pair(n);    /* F(n),   F(n+1) */
    seq_t lucas = pn1.f + pn.f_next;
    seq_t sq = lucas * lucas;

    if ((n & (idx_t)1ULL) != 0) {
        return sq;                      /* odd n: L(n)^2 */
    } else {
        return sq - (seq_t)2;           /* even n>0: L(n)^2-2 */
    }
}
