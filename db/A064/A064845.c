/*
 * A064845  Let r_1 = 1; r_{n+1} = [r_1; r_2, r_3,..., r_n]; n-th term is
 *          numerator of r_n.
 * https://oeis.org/A064845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A064845 – numerator of r_n defined by
 *      r_1 = 1
 *      r_{n+1} = [r_1; r_2, r_3, …, r_n]  (continued fraction)
 *  a(n) is the numerator of r_n.
 *
 *  The function below evaluates each r_k in turn, storing the
 *  reduced numerator and denominator.  The recurrence is
 *      r_2 = r_1,
 *      r_{k} = 1 + 1 / [r_2; r_3, …, r_{k-1}]   for k ≥ 3
 *
 *  Each new r_k is obtained by starting from the tail value v = r_{k-1}
 *  and working backward over r_{k-2}, …, r_2, updating
 *          v = r_j + 1/v
 *  until the first term r_1 is added as 1 + 1/v.
 *  The fraction is reduced after each update to keep the numbers as
 *  small as possible.
 *  The static arrays memoise all calculated terms so that any call
 *  order works efficiently (overall O(n²) arithmetic operations).
 */

#include <stdlib.h>

/* Euclidean algorithm for seq_t (works for uint8_t … unsigned __int128
   and for arbitrary‑precision types that support % and ==). */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Return the numerator a(n) of r_n.  The function memoises all
 * intermediate values, so calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range – not used */

    /* Static memoisation storage */
    static seq_t *num = NULL;   /* numerators */
    static seq_t *den = NULL;   /* denominators */
    static idx_t cap  = 0;      /* current array capacity (>=1) */
    static idx_t maxn = 0;      /* largest n already computed */

    /* Ensure capacity for indices up to n (1‑based). */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        num = (seq_t *)realloc(num, new_cap * sizeof(seq_t));
        den = (seq_t *)realloc(den, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Initialise r_1 if it has not yet been set. */
    if (maxn == 0) {
        num[1] = 1;
        den[1] = 1;
        maxn   = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = maxn + 1; i <= n; ++i) {
        if (i == 2) {                /* r_2 = r_1 */
            num[2] = num[1];
            den[2] = den[1];
            continue;
        }

        /* Start value v = r_{i-1}. */
        seq_t v_num = num[i-1];
        seq_t v_den = den[i-1];

        /* Work backwards through r_{i-2}, …, r_2. */
        for (idx_t k = i-2; k >= 2; --k) {
            /* v = r_k + 1/v
             *   = num[k]/den[k] + v_den/v_num
             *   = (num[k]*v_num + v_den*den[k]) / (den[k]*v_num) */
            seq_t new_num = num[k]*v_num + v_den*den[k];
            seq_t new_den = den[k]*v_num;

            /* Reduce to keep numbers small. */
            seq_t g = seq_gcd(new_num, new_den);
            new_num /= g;
            new_den /= g;

            v_num = new_num;
            v_den = new_den;
        }

        /* Now r_i = 1 + 1/v
         *     = (v_num + v_den) / v_num */
        seq_t num_i = v_num + v_den;
        seq_t den_i = v_num;

        /* Reduce final fraction. */
        seq_t g = seq_gcd(num_i, den_i);
        num_i /= g;
        den_i /= g;

        num[i] = num_i;
        den[i] = den_i;
    }

    maxn = n;
    return num[n];
}
