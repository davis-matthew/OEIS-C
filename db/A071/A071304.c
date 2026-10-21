/*
 * A071304  a(n) = (1/2) * (number of n X n 0..4 matrices M with MM' mod 5 =
 *          I, where M' is the transpose of M and I is the n X n identity
 *          matrix).
 * https://oeis.org/A071304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * |{M ∈ GF(5)^(n×n) | MMᵀ = I}|
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For odd n = 2k+1:  a(n) = 5^k * ∏_{i=0}^{k-1} (5^(2k) – 5^(2i)).  */
/* For even n = 2k: a(n) = (5^k – 1) * ∏_{i=1}^{k-1} (5^(2k) – 5^(2i)). */

#include <stdlib.h>

static seq_t *pow5 = NULL;      /* pow5[e] = 5^e */
static size_t pow5_len = 0;     /* number of stored entries */

/* Ensure that pow5[0..n] are available (grows using realloc). */
static void ensure_pow5(idx_t n)
{
    size_t needed = (size_t)n + 1;      /* indices 0 .. n inclusive */

    if (pow5_len >= needed) return;    /* already sufficient */

    seq_t *new_pow = NULL;

    if (pow5 == NULL) {
        new_pow = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!new_pow) exit(EXIT_FAILURE);
        new_pow[0] = (seq_t)1;
        for (size_t i = 1; i < needed; ++i)
            new_pow[i] = new_pow[i-1] * (seq_t)5;
    } else {
        new_pow = (seq_t *)realloc(pow5, needed * sizeof(seq_t));
        if (!new_pow) exit(EXIT_FAILURE);
        for (size_t i = pow5_len; i < needed; ++i)
            new_pow[i] = new_pow[i-1] * (seq_t)5;
    }

    pow5     = new_pow;
    pow5_len = needed;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for offset < 1 */

    ensure_pow5((n < 2) ? 2 : (2 * (n / 2)) + 2);  /* need at least 5^(2k) */

    if (n == 1) return (seq_t)1;

    if (n & 1U) {                         /* odd: n = 2k+1 */
        idx_t k   = (n - 1) / 2;
        seq_t res = pow5[k];
        idx_t full = 2 * k;               /* exponent for 5^(2k) */

        for (idx_t i = 0; i < k; ++i) {
            seq_t term = pow5[full] - pow5[2 * i];
            res *= term;
        }
        return res;
    }

    /* even: n = 2k */
    idx_t k   = n / 2;
    seq_t res = pow5[k] - (seq_t)1;
    if (k <= 1) return res;              /* product over empty range */

    idx_t full = 2 * k;

    for (idx_t i = 1; i < k; ++i) {
        seq_t term = pow5[full] - pow5[2 * i];
        res *= term;
    }
    return res;
}
