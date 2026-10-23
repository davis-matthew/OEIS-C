/*
 * A192335  Numbers n such that phi(n) divides rad(n).
 * https://oeis.org/A192335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that phi(n) divides rad(n). */
/* The sequence is finite: 1, 2, 4, 6, 18. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: compute smallest positive exponent of 2 in x.  */
static int exp_of_2(seq_t n)
{
    int e = 0;
    while ((n & 1) == 0 && n != 0) {
        e++;
        n >>= 1;
    }
    return e;
}

/* Helper: compute positive exponent of 3 in n (assuming n%3==0 if exponent).  */
static int exp_of_3(seq_t n)
{
    int e = 0;
    while (n % 3u == 0 && n != 0) {
        e++;
        n /= 3;
    }
    return e;
}

/* Cached terms.  */
static seq_t cached_terms[5] = {0};
static size_t cached_cnt = 0;
static bool initialized = false;

/* Initialize cache on first call.  */
static void init_cache(void)
{
    if (initialized) return;
    cached_cnt = 0;

    /* Candidates: 2^a * 3^b with 0 <= a,b <= 2.  */
    for (int a = 0; a <= 2; ++a) {
        seq_t pow2 = 1;
        for (int i = 0; i < a; ++i) pow2 *= 2;
        for (int b = 0; b <= 2; ++b) {
            seq_t pow3 = 1;
            for (int i = 0; i < b; ++i) pow3 *= 3;
            seq_t n = pow2 * pow3;

            /* Compute rad(n): product of distinct primes dividing n.  */
            seq_t rad = 1;
            if (n % 2u == 0) rad *= 2;
            if (n % 3u == 0) rad *= 3;

            /* Compute phi(n).  */
            int e2 = exp_of_2(n);
            int e3 = exp_of_3(n);

            seq_t phi = 1;
            if (e2 > 0) {
                /* phi(2^e2) = 2^(e2-1)  */
                if (e2 == 1)
                    phi *= 1;
                else
                    phi *= (seq_t)1 << (e2 - 1);
            }
            if (e3 > 0) {
                /* phi(3^e3) = 2 * 3^(e3-1)  */
                seq_t pow3phi = 1;
                for (int i = 0; i < e3 - 1; ++i) pow3phi *= 3;
                phi *= 2 * pow3phi;
            }

            /* Check divisibility.  */
            if (phi != 0 && rad % phi == 0) {
                if (cached_cnt < 5) {
                    cached_terms[cached_cnt++] = n;
                }
            }
        }
    }

    /* Sort ascending.  Simple selection sort; only 5 elements.  */
    for (size_t i = 0; i + 1 < cached_cnt; ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < cached_cnt; ++j) {
            if (cached_terms[j] < cached_terms[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            seq_t tmp = cached_terms[i];
            cached_terms[i] = cached_terms[min_idx];
            cached_terms[min_idx] = tmp;
        }
    }
    initialized = true;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    init_cache();
    if (n == 0 || n > cached_cnt) return 0;
    return cached_terms[n - 1];
}
