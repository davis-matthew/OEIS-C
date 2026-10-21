/*
 * A088858  Define a Fibonacci-type sequence to be one of the form s(0) = s_1
 *          >= 1, s(1) = s_2 >= 1, s(n+2) = s(n+1) + s(n); then a(n) = maximal
 *          m such that n is the m-th term in some Fibonacci-type sequence.
 * https://oeis.org/A088858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximal m such that n is the m-th term of some
 *            Fibonacci-type sequence. The implementation follows the
 *            characterization in Theorem 2.12 (Jones & Kiss): For n>1, a(n)
 *            is the largest integer r>1 satisfying ceil(((-1)^r F(r-2) n +
 *            1)/F(r-1)) ≤ floor(((-1)^r F(r-1) n – 1)/F(r)). The program
 *            ev...
 * Range:     u8 n <= 306, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1. */
    if (n == 1) {
        return 1;
    }

    /* Precompute Fibonacci numbers up to F(100).
       F(100) = 354224848179261915075 < 2^128, so it fits into any seq_t used in the tests. */
    #define MAX_R 100
    static seq_t fib[MAX_R + 1];
    static int initialized = 0;
    if (!initialized) {
        fib[0] = 0;
        fib[1] = 1;
        for (size_t i = 2; i <= MAX_R; ++i) {
            fib[i] = fib[i - 1] + fib[i - 2];
        }
        initialized = 1;
    }

    seq_t best = 1;         /* At least m=1 is always possible. */
    for (size_t r = 2; r <= MAX_R; ++r) {
        seq_t a_fib = fib[r - 2];
        seq_t b_fib = fib[r - 1];
        seq_t c_fib = fib[r];

        int ok; /* true if the inequality holds for this r. */

        if ((r & 1) == 0) {           /* r is even */
            /* left = ceil((a_fib * n + 1) / b_fib) */
            __int128 left_num = (__int128)a_fib * (__int128)n + 1;
            seq_t left = (seq_t)((left_num + (__int128)b_fib - 1) / (__int128)b_fib);

            /* right = floor((b_fib * n - 1) / c_fib) */
            __int128 right_num = (__int128)b_fib * (__int128)n - 1;
            seq_t right = (seq_t)(right_num / (__int128)c_fib);

            ok = (left <= right);
        } else {                       /* r is odd */
            /* left = floor((a_fib * n - 1) / b_fib) */
            __int128 left_num = (__int128)a_fib * (__int128)n - 1;
            seq_t left = (seq_t)(left_num / (__int128)b_fib);

            /* right = ceil((b_fib * n + 1) / c_fib) */
            __int128 right_num = (__int128)b_fib * (__int128)n + 1;
            seq_t right = (seq_t)((right_num + (__int128)c_fib - 1) / (__int128)c_fib);

            ok = (left >= right);
        }

        if (!ok) {
            break;  /* current r does not work; best is the previous r-1 */
        } else {
            best = (seq_t)r;
        }
    }

    return best;
}
