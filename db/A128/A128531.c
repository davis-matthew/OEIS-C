/*
 * A128531  a(n) = numerator of r(n): r(n) is such that the continued fraction
 *          (of rational terms) [r(1);r(2),...r(n)] equals the n-th Fibonacci
 *          number, for every positive integer n.
 * https://oeis.org/A128531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(n) = numerator of r(n) where r(1)=1, r(2)=1, r(3)=-2, and for
 *            n>=4, r(n) = -F(n)/(F(n-3)*r(n-1)) (F(k) is the k‑th Fibonacci
 *            number)
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 13, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Cache for numerator and denominator of r(i) (i starting at 1) */
    static seq_t *num = NULL, *den = NULL;
    static idx_t  cached_n = 0;      /* number of terms currently cached       */
    static size_t cache_cap = 0;     /* allocated capacity for num and den      */

    /* Cache for Fibonacci numbers, fibs[i] = F(i) (F(0)=0,F(1)=1) */
    static seq_t *fibs = NULL;
    static idx_t  fib_cached = 1;    /* highest Fibonacci index computed       */
    static size_t fib_cap = 0;       /* allocated capacity for fibs            */

    /* ---------- initialise caches if necessary ---------- */
    if (cached_n == 0) {
        size_t cap = (n > 64) ? n + 16 : 64;
        num = (seq_t *)malloc(cap * sizeof(seq_t));
        den = (seq_t *)malloc(cap * sizeof(seq_t));
        cache_cap = cap;

        /* Initial terms of r(n) */
        num[0] =  1;  den[0] =  1;    /* r(1)=1/1 */
        num[1] =  1;  den[1] =  1;    /* r(2)=1/1 */
        num[2] = -2;  den[2] =  1;    /* r(3)=-2/1 */
        cached_n = 3;
    }

    /* ---------- ensure Fibonacci numbers up to n available ---------- */
    if (n > fib_cached) {
        if (fibs == NULL) {
            size_t cap = (n > 64) ? n + 16 : 64;
            fibs = (seq_t *)malloc(cap * sizeof(seq_t));
            fib_cap = cap;
            fibs[0] = 0;       /* F(0) */
            fibs[1] = 1;       /* F(1) */
            fib_cached = 1;
        }
        if ((size_t)(n + 1) > fib_cap) {
            size_t cap = fib_cap * 2;
            if (cap < (size_t)(n + 1)) cap = n + 1;
            fibs = (seq_t *)realloc(fibs, cap * sizeof(seq_t));
            fib_cap = cap;
        }
        for (idx_t i = fib_cached + 1; i <= n; ++i)
            fibs[i] = fibs[i - 1] + fibs[i - 2];
        fib_cached = n;
    }

    /* ---------- compute r(n) terms up to n if not already cached ---------- */
    if (n > cached_n) {
        if ((size_t)n > cache_cap) {              /* enlarge capacity if needed */
            size_t cap = cache_cap ? cache_cap * 2 : 64;
            if (cap < (size_t)n) cap = n;
            num = (seq_t *)realloc(num, cap * sizeof(seq_t));
            den = (seq_t *)realloc(den, cap * sizeof(seq_t));
            cache_cap = cap;
        }

        for (idx_t i = cached_n; i < n; ++i) {
            idx_t term = i + 1; /* term number   */
            if (term < 4) continue; /* already known */

            seq_t Fk   = fibs[term];
            seq_t Fk_3 = fibs[term - 3];

            seq_t prev_num = num[i - 1];
            seq_t prev_den = den[i - 1];

            /* Cancel common factors early to avoid overflow */
            seq_t g1 = seq_gcd(Fk, prev_num);
            Fk   /= g1;
            prev_num /= g1;

            seq_t g2 = seq_gcd(prev_den, Fk_3);
            prev_den /= g2;
            Fk_3 /= g2;

            /* Now compute new numerator and denominator */
            /* Use 128‑bit intermediate to avoid 64‑bit overflow */
            __int128 tmp_num = (__int128)Fk * (__int128)prev_den;
            __int128 tmp_den = (__int128)Fk_3 * (__int128)prev_num;

            seq_t new_num = (seq_t)(-tmp_num);
            seq_t new_den = (seq_t)tmp_den;

            /* Further reduce if any common factors remain */
            seq_t g3 = seq_gcd(new_num, new_den);
            if (g3 != 0) {
                new_num /= g3;
                new_den /= g3;
            }

            /* Ensure denominator positive */
            if (new_den < 0) {
                new_den = -new_den;
                new_num = -new_num;
            }

            num[i] = new_num;
            den[i] = new_den;
        }
        cached_n = n;
    }

    return num[n - 1];
}
