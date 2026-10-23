/*
 * A117000  a(n) = Sum_{d|n} Jacobi(2,d)*d.
 * https://oeis.org/A117000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} Jacobi(2,d) * d
 * Range:     i8 n <= 118, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Jacobi(2,d) = 0 if d even, 1 if d ≡ ±1 (mod 8), –1 if d ≡ ±3 (mod 8) */

#include <stdint.h>
#include <stddef.h>

/* Note: seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n=0, safe anyway */

    /* Factorise n by trial division.  n ≤ 10000, so this is fast. */
    uint64_t temp = (uint64_t)n;
    int primes[128];
    int exps[128];
    int pc = 0;

    /* handle factor 2 */
    if ((temp & 1ULL) == 0) {
        int e = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            e++;
        }
        primes[pc] = 2;
        exps[pc]   = e;
        pc++;
    }

    /* odd factors, iterate only over odd numbers */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[pc] = (int)p;
            exps[pc]   = e;
            pc++;
        }
    }
    if (temp > 1) {
        primes[pc] = (int)temp;
        exps[pc]   = 1;
        pc++;
    }

    /* Build all divisors of n.  They fit into a small fixed array. */
    idx_t divisors[2048];
    size_t divcnt = 1;
    divisors[0] = 1;

    for (int i = 0; i < pc; ++i) {
        size_t cur = divcnt;
        int p = primes[i];
        int e = exps[i];
        uint64_t mul = 1;
        for (int k = 1; k <= e; ++k) {
            mul *= (uint64_t)p;
            for (size_t j = 0; j < cur; ++j) {
                divisors[divcnt++] = (idx_t)(divisors[j] * mul);
            }
        }
    }

    /* Sum d * Jacobi(2,d) */
    seq_t sum = 0;
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t d = divisors[i];
        int js;
        if ((d & 1ULL) == 0) {
            js = 0;                     /* even d */
        } else {
            uint8_t r = d % 8;
            js = (r == 1 || r == 7) ? 1 : -1;
        }
        sum += ((seq_t)d) * ((seq_t)js);
    }

    return sum;
}
