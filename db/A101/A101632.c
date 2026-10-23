/*
 * A101632  Denominator of partial sums of a certain series.
 * https://oeis.org/A101632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator(24 * Σ_{k=1..n} 1/((5k-1)*k*(5k+1)))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Greatest‑common divisor for seq_t (usable in both C and C++ builds) */
static inline seq_t gcd_seq(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

#ifdef __cplusplus
/* C++ build – seq_t may be an arbitrary‑precision integer type.  */
seq_t a(idx_t n)
{
    const seq_t SCALE = 24;
    seq_t num = 0;          /* numerator of the partial sum */
    seq_t den = 1;          /* denominator  */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t five_k = 5 * kseq;
        seq_t term_den = (five_k - 1) * kseq * (five_k + 1);  /* (5k-1)·k·(5k+1) */

        seq_t g = gcd_seq(den, term_den);
        seq_t l = den / g;
        seq_t r = term_den / g;

        /* add the k‑th term: 24/term_den */
        seq_t sum_num = num * r + SCALE * l;
        seq_t sum_den = den * r;

        seq_t h = gcd_seq(sum_num, sum_den);
        num = sum_num / h;
        den = sum_den / h;
    }
    return den;
}
#else
/* C build – seq_t is an unsigned integer type (uint8_t … uint64_t).  */
/* Use 128‑bit temporaries to avoid overflows in intermediate products.  */
#include <inttypes.h>

static inline __uint128_t
gcd128(__uint128_t a, __uint128_t b)
{
    while (b != 0) {
        __uint128_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    const seq_t SCALE = 24;
    seq_t num = 0;          /* numerator of the partial sum */
    seq_t den = 1;          /* denominator  */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t five_k = 5 * kseq;
        seq_t term_den = (five_k - 1) * kseq * (five_k + 1);  /* (5k-1)·k·(5k+1) */

        seq_t g  = gcd_seq(den, term_den);
        seq_t l  = den / g;
        seq_t r  = term_den / g;

        /* add the k-th term: 24/term_den */
        __uint128_t tmp_num = (__uint128_t)num * r + (__uint128_t)SCALE * l;
        __uint128_t tmp_den = (__uint128_t)den * r;

        __uint128_t h = gcd128(tmp_num, tmp_den);
        num = (seq_t)(tmp_num / h);
        den = (seq_t)(tmp_den / h);
    }
    return den;
}
#endif
