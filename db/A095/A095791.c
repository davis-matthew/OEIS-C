/*
 * A095791  Number of digits in lazy-Fibonacci-binary representation of n
 *          (A104326).
 * https://oeis.org/A095791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(floor(n/phi)) + 1 (phi = (1+√5)/2)
 *            The function computes a(n) by iteratively applying floor(n/phi)
 *            using Fibonacci approximations
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9895 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------- helper: floor(n / phi) with integer arithmetic -------- */
static uint64_t floor_div_phi(uint64_t n) {
    /* Fibonacci numbers stored in unsigned __int128 for safety */
    static unsigned __int128 fib[95];
    static bool init = false;
    if (!init) {
        fib[0] = 0;
        fib[1] = 1;
        for (int i = 2; i < 95; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];
        init = true;
    }

    /* find smallest i such that fib[i] > n; then use indices i-1 (numerator) and i (denominator) */
    int lo = 1, hi = 94;                     /* fib[94] > 2^64-1, so bounds are safe */
    while (lo < hi) {
        int mid = (lo + hi) >> 1;
        if (fib[mid] > n)
            hi = mid;
        else
            lo = mid + 1;
    }
    /* lo points to first fib[lo] > n */
    unsigned __int128 numer = fib[lo - 1];
    unsigned __int128 denom = fib[lo];
    unsigned __int128 prod  = (unsigned __int128)n * numer;
    return (uint64_t)(prod / denom);
}

/* ------------ sequence function -------------------------------- */
seq_t a(idx_t n) {
    /* base cases */
    if (n == 0 || n == 1)
        return 1;

    uint64_t cur = (uint64_t)n;
    uint64_t cnt = 0;
    while (cur >= 2) {
        cur = floor_div_phi(cur);
        ++cnt;
    }
    return (seq_t)(cnt + 1);
}
