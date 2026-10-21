/*
 * A063933  Difference between n and the average of largest prime less than or
 *          equal to n and smallest prime greater than or equal to n.
 * https://oeis.org/A063933
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - (p_prev(n) + p_next(n)) / 2 where p_prev(n) is the
 *            largest prime ≤ n and p_next(n) is the smallest prime ≥ n (a(n)
 *            = 0 when n is prime)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Simple deterministic primality test for 64‑bit integers.
   Uses 6k±1 wheel.  Suitable for n up to about 10^12. */
static bool is_prime(idx_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    idx_t i = 5;
    for (;;) {
        /* i*i may overflow 64‑bit, so use __int128 */
        __int128 sq = (__int128)i * i;
        if (sq > n) break;
        if (n % i == 0 || n % (i + 2) == 0) return false;
        i += 6;
    }
    return true;
}

static idx_t prev_prime(idx_t n) {
    /* n ≥ 2 in our usage */
    while (!is_prime(n)) n--;
    return n;
}

static idx_t next_prime(idx_t n) {
    /* n ≥ 2 in our usage */
    while (!is_prime(n)) n++;
    return n;
}

seq_t a(idx_t n) {
    /* Offset 2: n≥2, but function works for any n≥2 */
    idx_t lp = prev_prime(n);   /* largest prime ≤ n */
    idx_t np = next_prime(n);   /* smallest prime ≥ n */
    seq_t avg = ((seq_t)lp + (seq_t)np) / 2;
    return (seq_t)n - avg;
}
