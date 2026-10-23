/*
 * A199660  Number of parity alternating permutations of [n] avoiding descents
 *          from odd to even numbers.
 * https://oeis.org/A199660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(2*n) = (2^n + n - 1) * (n-1)! for n>0; a(2*n+1) = n!
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 21, u64 n <= 35, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *factorial_cache = NULL;   /* factorial_cache[i] == i!   */
static idx_t  factorial_len  = 0;       /* next index to compute  */
static seq_t *pow2_cache     = NULL;    /* pow2_cache[i]     == 2^i   */
static idx_t  pow2_len       = 0;       /* next index to compute  */

static void ensure_factorial(idx_t upto)
{
    if (factorial_len > upto) return;
    size_t new_len = (size_t)(upto + 1);
    if (new_len < 2 * factorial_len) new_len = 2 * factorial_len;
    if (new_len == 0) new_len = 1;

    seq_t *tmp = (seq_t*)realloc(factorial_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    factorial_cache = tmp;

    if (factorial_len == 0) {
        factorial_cache[0] = (seq_t)1;   /* 0! = 1 */
        factorial_len = 1;
    }

    for (idx_t i = factorial_len; i <= upto; ++i) {
        factorial_cache[i] = factorial_cache[i-1] * (seq_t)i;
    }
    factorial_len = upto + 1;
}

static void ensure_pow2(idx_t upto)
{
    if (pow2_len > upto) return;
    size_t new_len = (size_t)(upto + 1);
    if (new_len < 2 * pow2_len) new_len = 2 * pow2_len;
    if (new_len == 0) new_len = 1;

    seq_t *tmp = (seq_t*)realloc(pow2_cache, new_len * sizeof(seq_t));
    if (!tmp) abort();
    pow2_cache = tmp;

    if (pow2_len == 0) {
        pow2_cache[0] = (seq_t)1;        /* 2^0 = 1 */
        pow2_len = 1;
    }

    for (idx_t i = pow2_len; i <= upto; ++i) {
        pow2_cache[i] = pow2_cache[i-1] * (seq_t)2;
    }
    pow2_len = upto + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t k = n >> 1;                  /* k = floor(n/2) */

    if (n & 1) {                       /* odd: a(2k+1) = k! */
        ensure_factorial(k);
        return factorial_cache[k];
    }

    /* even (n > 0): a(2k) = (2^k + k - 1) * (k-1)! */
    ensure_factorial(k - 1);
    ensure_pow2(k);

    seq_t result = (pow2_cache[k] + (seq_t)k - (seq_t)1) * factorial_cache[k - 1];
    return result;
}
