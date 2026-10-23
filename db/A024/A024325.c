/*
 * A024325  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = A023531, t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A024325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..floor((n+1)/2)} A023531(j)*A001950(n-j+1)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Upper Wythoff sequence cache: T(i) = floor(i*φ)+i  (i ≥ 0) */
static seq_t *t_cache = NULL;
static idx_t t_cache_size = 0;

/* Ensure t_cache has entries up to index n (inclusive).  */
static void ensure_t_cache(idx_t n) {
    if (t_cache_size > n)
        return;                                 /* already sufficient */

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(t_cache, new_size * sizeof(seq_t));
    if (!new_cache)                               /* allocation failed */
        return;                                   /* keep old cache */

    t_cache = new_cache;
    for (idx_t i = t_cache_size; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t sq5   = floor_sqrt(i_seq * i_seq * (seq_t)5);   /* floor(i*√5) */
        seq_t p     = (i_seq + sq5) >> 1;                    /* floor(i*φ) */
        t_cache[i]  = i_seq + p;                             /* T(i) */
    }
    t_cache_size = new_size;
}

/* A023531(j) = 1  iff  j = k*(k+3)/2  for some k ≥ 1.  */
static bool s_is_one(idx_t j) {
    if (j == 0)
        return false;

    seq_t d = (seq_t)9 + (seq_t)8 * j;              /* d = 8j + 9 = (2k+3)² */
    seq_t s = floor_sqrt(d);
    return (s * s == d) && (((s - 3) & 1) == 0);    /* (s-3) even ⇒ integer k */
}

seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0;

    ensure_t_cache(n);

    idx_t k      = (n + 1) >> 1;                    /* floor((n+1)/2)   */
    seq_t result = (seq_t)0;

    for (idx_t j = 1; j <= k; ++j) {
        if (s_is_one(j))
            result += t_cache[n - j + 1];
    }
    return result;
}
