/*
 * A194738  Number of k such that {k*sqrt(3)} < {n*sqrt(3)}, where { } =
 *          fractional part.
 * https://oeis.org/A194738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0 ≤ k < n with {k*sqrt(3)} < {n*sqrt(3)}
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Unsigned integer square root for a 128‑bit value.
   Returns the floor of sqrt(x). */
static inline uint64_t isqrt_u128(unsigned __int128 x) {
    unsigned __int128 lo = 0, hi = (((unsigned __int128)1) << 64) - 1;
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq == x) return (uint64_t)mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return (uint64_t)hi;
}

/* Dynamic arrays for floor(k*sqrt(3)) and a(n) values. */
static size_t floor_size = 0;          /* number of entries in floorS (0‑indexed) */
static size_t a_size    = 0;          /* largest computed a(n) index + 1 */
static uint64_t *floorS = NULL;       /* floorS[k] = floor(k*sqrt(3)) */
static seq_t   *a_seq   = NULL;       /* a_seq[n] = a(n) */

/* Ensure that floorS has at least 'new_cap' entries and fill new ones. */
static void ensure_floor_size(size_t new_cap) {
    if (new_cap <= floor_size) return;
    size_t old_cap  = floor_size;
    size_t new_size = new_cap;
    uint64_t *tmp = (uint64_t *)realloc(floorS, new_size * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    floorS = tmp;
    for (size_t k = old_cap; k < new_size; ++k) {
        unsigned __int128 prod = (unsigned __int128)3 * k * k; /* 3*k*k */
        floorS[k] = isqrt_u128(prod);                       /* floor(k*sqrt(3)) */
    }
    floor_size = new_size;
}

/* Ensure that a_seq has at least 'new_cap' entries and compute new values. */
static void ensure_a_size(size_t new_cap) {
    if (new_cap <= a_size) return;
    size_t old_cap  = a_size;
    size_t new_size = new_cap;
    seq_t *tmp = (seq_t *)realloc(a_seq, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_seq = tmp;
    for (size_t n = old_cap; n < new_size; ++n) {
        uint64_t count = 0;
        uint64_t floor_n = floorS[n];
        for (size_t k = 0; k < n; ++k) {
            uint64_t d     = n - k;                /* n - k > 0 */
            uint64_t diff  = floor_n - floorS[k];  /* floor difference */
            unsigned __int128 lhs = (__int128)3 * d * d;      /* 3*d^2 */
            unsigned __int128 rhs = (__int128)diff * diff;    /* diff^2 */
            if (rhs < lhs)                          /* diff^2 < 3*d^2 */
                ++count;
        }
        a_seq[n] = (seq_t)count;
    }
    a_size = new_size;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1 in OEIS */
    /* We need floor(k*sqrt(3)) for k up to n. */
    ensure_floor_size((size_t)n + 1);
    /* Compute a(n) and all previous values. */
    ensure_a_size((size_t)n + 1);
    return a_seq[(size_t)n];
}
