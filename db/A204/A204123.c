/*
 * A204123  Symmetric matrix based on f(i,j)=max([i/j],[j/i]), where [
 *          ]=floor, by antidiagonals.
 * https://oeis.org/A204123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = max(floor(i/j), floor(j/i)) where (i,j) * correspond to
 *            the n‑th entry in the matrix * M(i,j)=max(floor(i/j),floor(j/i))
 *            enumerated by anti‑diagonals * starting from (1,1)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned long long isqrt128(unsigned __int128 v) {
    unsigned long long low = 0;
    unsigned long long high = ((unsigned long long)1) << 35; /* > sqrt(max 1+8n) */
    while (low < high) {
        unsigned long long mid = low + ((high - low) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= v)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1; /* floor(sqrt(v)) */
}

seq_t a(idx_t n) {
    /* Step 1: find diagonal index t such that
       t*(t+1)/2 >= n  and  (t-1)*t/2 < n. */
    unsigned __int128 target = (unsigned __int128)8 * n + 1;
    unsigned long long s = isqrt128(target);   /* floor sqrt */
    unsigned long long t = (s - 1) / 2;         /* floor */
    while ((unsigned __int128)t * (t + 1) / 2 < n) ++t;

    /* Step 2: compute i and j for this diagonal */
    unsigned __int128 prev_tri = (unsigned __int128)(t - 1) * t / 2;
    idx_t offset = n - (idx_t)prev_tri - 1;     /* 0‑based position in diag */
    idx_t i = offset + 1;
    idx_t j = t + 1 - i;

    /* Step 3: compute the matrix value */
    idx_t lo = i / j;
    idx_t hi = j / i;
    seq_t val = lo > hi ? (seq_t)lo : (seq_t)hi;
    return val;
}
