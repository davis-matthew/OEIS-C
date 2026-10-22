/*
 * A000750  Expansion of bracket function.
 * https://oeis.org/A000750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   G.f. 1/((1+x)^5-x^5), i.e. a(n) = -5*a(n-1) - 10*a(n-2) -
 *            10*a(n-3) - 5*a(n-4), a(0) = 1, a(1) = -5, a(2) = 15, a(3) =
 *            -35. For fixed-width builds this runs modulo 2^128 (or 2^64), so
 *            every value that fits is exact even if earlier terms overflow
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 31, i64 n <= 65, i128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#ifdef __cplusplus
#include <limits>
#endif

static seq_t a750_rec_seq(idx_t n) {
    seq_t w0 = 1, w1 = 0, w2 = 0, w3 = 0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t t = 0;
        t -= w0 * 5;
        t -= w1 * 10;
        t -= w2 * 10;
        t -= w3 * 5;
        w3 = w2;
        w2 = w1;
        w1 = w0;
        w0 = t;
    }
    return w0;
}

static unsigned __int128 a750_rec_mod(idx_t n) {
    unsigned __int128 w0 = 1, w1 = 0, w2 = 0, w3 = 0;
    idx_t i;
    for (i = 1; i <= n; i++) {
        unsigned __int128 t = 0;
        t -= w0 * 5;
        t -= w1 * 10;
        t -= w2 * 10;
        t -= w3 * 5;
        w3 = w2;
        w2 = w1;
        w1 = w0;
        w0 = t;
    }
    return w0;
}

seq_t a(idx_t n) {
#ifdef __cplusplus
    if (std::numeric_limits<seq_t>::is_specialized &&
        std::numeric_limits<seq_t>::digits < 64) {
        uint64_t u = (uint64_t)a750_rec_mod(n);
        return (seq_t)(long long)u;
    }
    return a750_rec_seq(n);
#else
    (void)a750_rec_seq;
    return (seq_t)(__int128)a750_rec_mod(n);
#endif
}
