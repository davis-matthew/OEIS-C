/*
 * A070221  a(n) = gpf(n+1) - gpf(n), where gpf = A006530 (greatest prime
 *          factor).
 * https://oeis.org/A070221
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = gpf(n+1) – gpf(n) (gpf = A006530 = greatest prime factor)
 * Range:     i8 n <= 83, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Static storage for the largest‑prime‑factor table */
static idx_t *lpf = NULL;   /* lpf[i] = largest prime divisor of i  (i ≥ 2) */
static idx_t lpf_size = 0;  /* current allocated size (indices 0 … lpf_size-1) */

/* Ensure the lpf array is ready up to at least 'limit' (inclusive) */
static void ensure_lpf(idx_t limit)
{
    if (limit < 2)
        return;

    if (limit < lpf_size)   /* already computed up to limit */
        return;

    /* Allocate a new array large enough for 0 … limit */
    size_t new_size = (size_t)limit + 1;
    idx_t *new_lpf = (idx_t *)calloc(new_size, sizeof(idx_t));
    if (!new_lpf)                     /* allocation failed – keep old data */
        return;

    /* Simple sieve: for each prime p, set lpf[p], lpf[2p], lpf[3p], … = p
       (later, larger primes overwrite earlier ones, so the final value is
       the largest prime divisor) */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_lpf[i] == 0) {          /* i is prime */
            for (idx_t j = i; j <= limit; j += i) {
                new_lpf[j] = i;         /* overwrite with the larger prime */
            }
        }
    }

    /* Replace the old table */
    free(lpf);
    lpf = new_lpf;
    lpf_size = new_size;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)          /* the sequence is defined for n ≥ 2 only */
        return 0;

    /* We need gpf(n) and gpf(n+1) */
    ensure_lpf(n + 1);

    seq_t gp_n   = (seq_t)lpf[n];
    seq_t gp_np1 = (seq_t)lpf[n + 1];

    return gp_np1 - gp_n;
}
