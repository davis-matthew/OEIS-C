/*
 * A094102  Triangle read by rows in which row n contains Fib(1), ...,
 *          Fib(n-1), Fib(n), Fib(n-1), ..., Fib(1).
 * https://oeis.org/A094102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Fib(k) where k is the position of the n‑th element in the
 *            triangular arrangement Row r contains Fibonacci numbers
 *            Fib(1),…,Fib(r),…,Fib(1) (2r‑1 elements). For n≥1, let r =
 *            ceil(sqrt(n)), t = n – (r–1)² – 1 (t is the 0‑based index inside
 *            the row), k = t+1 if t < r, else k = 2r–1 – t. Then...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Auxiliary structure for fast doubling Fibonacci pair (F(k),F(k+1)). */
typedef struct { seq_t f0; seq_t f1; } FibPair;

/* Integer floor square root of an unsigned 64‑bit integer. */
static idx_t int_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 s = (unsigned __int128)mid * mid;
        if (s <= n) low = mid + 1;
        else high = mid - 1;
    }
    return high;      /* floor sqrt */
}

/* Fast doubling algorithm for Fibonacci numbers.  Returns
   (F(k),F(k+1)).  Works for any seq_t type. */
static FibPair fib_pair(idx_t k)
{
    if (k == 0) {
        FibPair zero = { (seq_t)0, (seq_t)1 };
        return zero;
    }
    FibPair p = fib_pair(k >> 1);
    seq_t a = p.f0;     /* F(k)   */
    seq_t b = p.f1;     /* F(k+1) */

    seq_t two_b = b + b;                     /* 2*F(k+1) */
    seq_t c = a * (two_b - a);               /* F(2k)   */
    seq_t d = a * a + b * b;                 /* F(2k+1) */

    if (k & 1) {
        FibPair odd = { d, c + d };          /* (F(2k+1),F(2k+2)) */
        return odd;
    } else {
        FibPair even = { c, d };             /* (F(2k),F(2k+1))   */
        return even;
    }
}

/* Return the k-th Fibonacci number (F(k)), 1‑based. */
static seq_t fib(idx_t k)
{
    return fib_pair(k).f0;
}

/* Main sequence function.  External linkage, exactly as required. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of spec, safety */

    /* Find row r so that (r-1)^2 < n ≤ r^2. */
    idx_t r = int_sqrt(n);
    if ((r * r) < n) ++r;                 /* ceil */

    idx_t previous = (r - 1) * (r - 1);
    idx_t t = n - previous - 1;           /* 0‑based offset in row */

    idx_t k;
    if (t < r) k = t + 1;                 /* ascending part */
    else       k = (2 * r - 1) - t;       /* descending part */

    return fib(k);
}
