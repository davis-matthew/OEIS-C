/*
 * A075829  Let u(1) = x and u(n+1) = (n^2/u(n)) + 1 for n >= 1; then a(n) is
 *          such that u(n) = (b(n)*x + c(n))/(d(n)*x + a(n)) (in lowest terms)
 *          and a(n), b(n), c(n), d(n) are positive integers.
 * https://oeis.org/A075829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator(∑_{k=1}^{n-1} (-1)^{k+1}/k) -
 *            numerator(∑_{k=1}^{n-1} (-1)^{k+1}/k) (in lowest terms)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#include "../utils/gcd.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Compute a(n) using the alternat­ing harmonic sum representation. */
seq_t a(idx_t n)
{
    /* static state holding the rational values of the alternating harmonic
       sums H_m = sum_{k=1}^m (-1)^{k+1}/k   (m = 0 .. ∞)  in lowest terms.
       H_0 = 0/1 is stored at index 0.  For m>=1, we store value at index m.   */
    static seq_t *num = NULL; /* numerator of H_m */
    static seq_t *den = NULL; /* denominator of H_m */
    static size_t cap = 0;    /* allocated length of arrays */
    static idx_t max_m = 0;   /* highest m computed (i.e. index of last element) */

    /* Helper: ensure arrays have at least 'need' slots (indices 0..need-1). */
    if (cap < (size_t)(max_m + 1)) {
        /* initialize storage for H_0 = 0/1 */
        size_t init_cap = 1;
        if (num == NULL) {
            num = seq_alloc_new_array(init_cap);
            den = seq_alloc_new_array(init_cap);
            num[0] = 0;
            den[0] = 1;
            cap = init_cap;
        }
    }
    /* Grow the arrays if necessary */
    if (cap < (size_t)(max_m + 1)) {
        size_t new_cap = (size_t)(max_m + 1);
        seq_t *new_num = seq_alloc_new_array(new_cap);
        seq_t *new_den = seq_alloc_new_array(new_cap);
        for (idx_t i = 0; i <= max_m; ++i) {
            new_num[i] = num[i];
            new_den[i] = den[i];
        }
        seq_free_array(num);
        seq_free_array(den);
        num = new_num;
        den = new_den;
        cap = new_cap;
    }

    /* We need H_{n-1}.  Compute all missing values up to that index. */
    idx_t target = n - 1;   /* because we store H_0..H_{n-1} for a(n) */
    while (max_m < target) {
        idx_t k = max_m + 1;          /* next index to compute : H_k */
        /* H_k = H_{k-1} + (-1)^{k+1}/k   in lowest terms  */
        seq_t prev_num = num[max_m];
        seq_t prev_den = den[max_m];
        seq_t numer = prev_num * (seq_t)k;
        if ((k & 1) == 0)          /* k even → subtract the term 1/k  */
            numer -= prev_den;
        else                       /* k odd  → add the term 1/k   */
            numer += prev_den;
        seq_t denom = prev_den * (seq_t)k;

        /* Reduce the fraction */
        seq_t g = gcd(numer, denom);
        numer /= g;
        denom /= g;

        /* Store the reduced value at index k */
        if (k >= (idx_t)cap) {
            /* reallocate one more slot if needed */
            size_t new_cap = (size_t)(k + 1);
            seq_t *new_num = seq_alloc_new_array(new_cap);
            seq_t *new_den = seq_alloc_new_array(new_cap);
            for (idx_t i = 0; i <= max_m; ++i) {
                new_num[i] = num[i];
                new_den[i] = den[i];
            }
            seq_free_array(num);
            seq_free_array(den);
            num = new_num;
            den = new_den;
            cap = new_cap;
        }
        num[k] = numer;
        den[k] = denom;
        max_m = k;
    }

    /* Return the difference of denominator and numerator for H_{n-1} */
    return den[target] - num[target];
}
