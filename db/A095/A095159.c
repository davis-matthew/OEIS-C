/*
 * A095159  Numerator of b(n) given by b(1) = 1, b(2) = 2; for n >= 3, b(n) =
 *          (-1)^n (2n-1) ((n-2)!!)^2/((n-1)!!)^2, where n!! is the double
 *          factorial A006882.
 * https://oeis.org/A095159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = (-1)^n * (2n-1) * ((n-2)!!)^2 / gcd(((n-2)!!)^2,
 *            ((n-1)!!)^2) where (k)!! denotes the double factorial. The
 *            function returns the numerators of the reduced fractions b(n)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 17, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   gcd, pow_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/pow_small.h"

static seq_t *dblfact = NULL;          /* array of double factorials */
static size_t dblfact_max = 0;         /* current maximum index computed */

/* Ensure dblfact[i] is computed for i <= n */
static void ensure_dblfact(idx_t n)
{
    if ((idx_t)dblfact_max >= n) return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *new_arr = (seq_t *)realloc(dblfact, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);          /* memory failure */
    dblfact = new_arr;

    if (dblfact_max == 0) {                    /* first allocation */
        dblfact[0] = 1;
        if (new_cap > 1) dblfact[1] = 1;
        dblfact_max = 1;
    }

    for (size_t i = dblfact_max + 1; i <= n; ++i) {
        if (i == 0 || i == 1) {
            dblfact[i] = 1;
        } else {
            dblfact[i] = ((seq_t)i) * dblfact[i - 2];
        }
    }
    dblfact_max = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    ensure_dblfact(n);

    seq_t a_val = dblfact[n - 2];
    seq_t b_val = dblfact[n - 1];

    seq_t g_ab = gcd(a_val, b_val);
    seq_t a1 = a_val / g_ab;
    seq_t b1 = b_val / g_ab;

    /* Compute g2 = gcd( (2n-1), b1^2 ) */
    int C_int = (int)((idx_t)2 * n - 1);
    int rem = C_int;
    seq_t b1_copy = b1;   /* copy to not destroy original */
    seq_t g2 = 1;

    for (int p = 2; (seq_t)p * (seq_t)p <= (seq_t)rem; ++p) {
        if (rem % p == 0) {
            int exp_c = 0;
            while (rem % p == 0) { rem /= p; exp_c++; }

            int exp_b1 = 0;
            while (b1_copy % p == 0) {
                b1_copy /= p;
                exp_b1++;
                /* early exit: if 2*exp_b1 >= exp_c, we already hit the max exponent for gcd */
                if (2 * exp_b1 >= exp_c) break;
            }
            int exp_gcd = (2 * exp_b1 >= exp_c) ? exp_c : (2 * exp_b1);
            g2 *= pow_small((seq_t)p, exp_gcd);
        }
    }
    if (rem > 1) {               /* residual prime factor */
        int p = rem;
        /* exponent in C is 1 */
        int exp_b1 = 0;
        while (b1_copy % p == 0) { b1_copy /= p; exp_b1++; }
        if (exp_b1 >= 1) g2 *= (seq_t)p;   /* exponent 1 */
    }

    seq_t C_div = ((seq_t)C_int) / g2;
    seq_t a1_sq = a1 * a1;
    seq_t result = C_div * a1_sq;

    /* Apply sign */
    if (n & 1) result = -result;   /* n odd -> negative */

    return result;
}
