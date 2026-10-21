/*
 * A074093  Number of values of k such that n = k - largest divisor of k (<k).
 * https://oeis.org/A074093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers k such that n = k – largest divisor of
 *            k (<k). Equivalently: count primes p with (p–1) | n and all
 *            prime factors of n/(p–1) are ≥ p. The value of k is then k =
 *            p·(n/(p–1))
 * Range:     u8 n <= 9945, u16 n <= 9945, u32 n <= 9945, u64 n <= 9945,
 *            u128 n <= 9945, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>  /* calloc, malloc, free, realloc */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n ≥ 1, but safe */

    /* ---------- generate all primes up to n+1 ---------- */
    idx_t limit = n + 1;
    bool *is_comp = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    if (!is_comp) return 0;              /* allocation failure */

    /* primes list (dynamic array) */
    size_t prime_cap = limit / 2 + 2;    /* rough upper bound */
    idx_t *primes = (idx_t *)malloc(prime_cap * sizeof(idx_t));
    if (!primes) { free(is_comp); return 0; }

    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            /* store prime */
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(primes, prime_cap * sizeof(idx_t));
                if (!tmp) { free(is_comp); free(primes); return 0; }
                primes = tmp;
            }
            primes[prime_cnt++] = i;

            /* mark multiples if i*i fits */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i) is_comp[j] = true;
            }
        }
    }
    free(is_comp);

    /* ---------- enumerate divisors of n ---------- */
    idx_t divs[260];          /* sqrt(10000) < 100, so 260 is safe */
    size_t div_cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[div_cnt++] = i;
            if (i != n / i) divs[div_cnt++] = n / i;
        }
    }

    /* ---------- count valid k's ---------- */
    seq_t count = 0;
    for (size_t d_idx = 0; d_idx < div_cnt; ++d_idx) {
        idx_t d = divs[d_idx];
        idx_t p = d + 1;               /* candidate prime */
        /* check if p is prime */
        bool p_is_prime = false;
        for (size_t i = 0; i < prime_cnt && primes[i] <= p; ++i) {
            if (primes[i] == p) { p_is_prime = true; break; }
        }
        if (!p_is_prime) continue;

        idx_t m = n / d;                /* m = n/(p-1) */
        bool ok = true;

        /* Quick reject: m > 1 and m < p means m's prime factor is < p */
        if (m > 1 && m < p) ok = false;
        else {
            /* test for any prime factor < p */
            for (size_t i = 0; i < prime_cnt && primes[i] < p && primes[i] <= m; ++i) {
                if (m % primes[i] == 0) { ok = false; break; }
            }
        }
        if (ok) ++count;
    }

    free(primes);
    return count;
}
