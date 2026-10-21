/*
 * A205972  a(n) = Fibonacci(n)*A122859(n) for n>=1, with a(0)=1, where
 *          A122859 lists the coefficients in phi(-q)^3/phi(-q^3) and phi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A205972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Fibonacci(n) * A122859(n), with a(0)=1. A122859(n) = 6 *
 *            Σ_{d|n} Kronecker(d,3) * (-1)^{n/d}
 * Range:     i8 n <= 6, i16 n <= 18, i32 n <= 41, i64 n <= 85, i128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>


/* Fast doubling Fibonacci: returns F(n).  */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);

    seq_t c = a * ((b * 2) - a);
    seq_t d = a * a + b * b;

    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

static seq_t fib_fast(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);
    return fn;
}

/* Generate all divisors from the prime factorisation. */
static void gen_divisors_rec(size_t idx, size_t pcnt,
                             uint64_t *pr, unsigned int *ex,
                             uint64_t cur, uint64_t *divs, size_t *dcnt)
{
    if (idx == pcnt) {
        divs[*dcnt] = cur;
        (*dcnt)++;
        return;
    }
    uint64_t p   = pr[idx];
    unsigned int e = ex[idx];
    for (unsigned int k = 0; k <= e; ++k) {
        gen_divisors_rec(idx + 1, pcnt, pr, ex, cur, divs, dcnt);
        cur *= p;
    }
}

/* Compute A122859(n) for n >= 1. */
static seq_t A122859(idx_t n)
{
    if (n == 0) return 0;

    /* Factor n. */
    static const size_t MAX_FACTORS = 32;
    uint64_t primes[MAX_FACTORS];
    unsigned int exps[MAX_FACTORS];
    size_t pcnt = 0;

    uint64_t temp = (uint64_t)n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            pcnt++;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        pcnt++;
    }

    /* Generate all divisors. */
    static const size_t MAX_DIVISORS = 4096;
    uint64_t divs[MAX_DIVISORS];
    size_t dcnt = 0;
    gen_divisors_rec(0, pcnt, primes, exps, 1, divs, &dcnt);

    /* Sum over divisors. */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d  = divs[i];
        uint64_t nd = (uint64_t)n / d;
        int sign = (nd & 1) ? -1 : 1;

        int kr;
        if (d % 3 == 0) {
            kr = 0;
        } else if (d % 3 == 1) {
            kr = 1;
        } else {
            kr = -1;
        }
        sum += (seq_t)(kr * sign);
    }
    return sum * (seq_t)6;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t fibn   = fib_fast(n);
    seq_t coeff  = A122859(n);
    return fibn * coeff;
}
