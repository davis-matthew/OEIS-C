/*
 * A204521  Square root of floor(A055812(n) / 5).
 * https://oeis.org/A204521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(floor(A055812(n) / 5))). The sequence can be
 *            generated from Fibonacci numbers: * If n % 4 == 2, let k =
 *            (n-2)/4; then a(n) = Fib(6*k) / 2. * Otherwise, let m be the
 *            number of integers i in [4, n] with i % 4 != 2. Then a(n) =
 *            Fib(2*m). Here Fib(t) denotes the t‑th Fibonacci numbe...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast doubling Fibonacci algorithm.
   Returns (Fib(n), Fib(n+1)) in *f_n and *f_n1. */
static void fib_pair(idx_t n, seq_t *f_n, seq_t *f_n1) {
    if (n == 0) {
        *f_n = 0;
        *f_n1 = 1;
        return;
    }
    idx_t half = n >> 1;
    seq_t a, b;
    fib_pair(half, &a, &b);

    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);
    seq_t d = a * a + b * b;

    if (n & 1) {
        *f_n  = d;          /* Fib(2k+1) = d */
        *f_n1 = c + d;      /* Fib(2k+2) */
    } else {
        *f_n  = c;          /* Fib(2k)   */
        *f_n1 = d;          /* Fib(2k+1) */
    }
}

seq_t a(idx_t n) {
    if (n <= 3)          /* a(1)=a(2)=a(3)=0 */
        return 0;

    if ((n & 3) == 2) {  /* n % 4 == 2 case */
        idx_t k = (n - 2) >> 2;           /* k = (n-2)/4 */
        idx_t fib_index = 6 * k;          /* 6*k */
        seq_t val, dummy;
        fib_pair(fib_index, &val, &dummy);  /* val = Fib(6*k) */
        return val / 2;
    } else {             /* n % 4 != 2 */
        /* Count numbers i in [4, n] that are NOT 2 modulo 4. */
        idx_t total = n - 3;               /* numbers from 4 to n inclusive */
        idx_t count_mod2 = 0;
        if (n >= 6) {
            count_mod2 = ((n - 6) >> 2) + 1; /* floor((n-6)/4)+1 */
        }
        idx_t m = total - count_mod2;
        idx_t fib_index = 2 * m;          /* because indices are even here */
        seq_t val, dummy;
        fib_pair(fib_index, &val, &dummy);  /* val = Fib(2*m) */
        return val;
    }
}
