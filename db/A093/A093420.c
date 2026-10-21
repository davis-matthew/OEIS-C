/*
 * A093420  Triangle read by rows: T(n,k) is the numerator of f(n, k) =
 *          (Product_{i = 0..k-1} (n-i))/(Sum_{i = 1..k} i) for 1 <= k <= n.
 * https://oeis.org/A093420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, tabl, less
 * Formula:   a(n) = numerator of (∏_{i=0}^{k-1} (r-i)) / (∑_{i=1}^{k} i),
 *            where r and k are chosen so that n = 1 + 2 + … + (r‑1) + k (row
 *            r, column k)
 * Range:     u8 n <= 32, u16 n <= 53, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* --- find row r and column k for the given index n --- */
    idx_t row = 0;
    idx_t sum = 0;                      /* sum_{i=1}^{row} i */
    while (sum < n) {
        row++;
        sum += row;
    }
    /* sum now holds C(row,2)+row = r(r+1)/2 >= n */
    idx_t k = n - (sum - row);         /* 1‑based column */

    /* --- compute denominator B = k(k+1)/2 and its prime factors --- */
    idx_t B = k * (k + 1) / 2;
    uint32_t primes[10];
    unsigned exp_f[10];
    int nf = 0;

    if (B > 0) {
        idx_t temp = B;

        /* factor 2 */
        if ((temp & 1) == 0) {
            unsigned e = 0;
            while ((temp & 1) == 0) { temp >>= 1; e++; }
            primes[nf] = 2;
            exp_f[nf] = e;
            nf++;
        }

        /* odd factors */
        for (idx_t p = 3; p * p <= temp; p += 2) {
            if (temp % p == 0) {
                unsigned e = 0;
                while (temp % p == 0) { temp /= p; e++; }
                primes[nf] = (uint32_t)p;
                exp_f[nf] = e;
                nf++;
            }
        }

        /* remaining prime */
        if (temp > 1) {
            primes[nf] = (uint32_t)temp;
            exp_f[nf] = 1;
            nf++;
        }
    }

    /* --- multiply the terms n, n-1, … , n-k+1, canceling the
         prime factors of B as we go --- */
    seq_t result = 1;
    unsigned rem[10];
    for (int i = 0; i < nf; ++i) rem[i] = exp_f[i];

    for (idx_t i = 0; i < k; ++i) {
        seq_t term = (seq_t)(row - i);
        for (int j = 0; j < nf; ++j) {
            while (rem[j] > 0 && term % (seq_t)primes[j] == 0) {
                term /= (seq_t)primes[j];
                rem[j]--;
            }
        }
        result *= term;
    }

    return result;
}
